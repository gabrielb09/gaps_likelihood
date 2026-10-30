#pragma once

#include <cmath>

#include "Th2.h"

#include "constants.hh"
#include "detector.hh"

namespace gaps_ml{

struct Landau{
    double location;
    double width;
};

inline Landau landau_pars(const double beta, const double angle_factor, const Detector& target, const double charge = 1){
    // angle_factor is either cos(theta) or sin(theta) depending on your detector orienation and
    // definition of theta. I think the most normal thing would be to define it as cos(theta)
    // for TKR, UMB, and CBE top and bottom and sin(theta) for everyhting else. In that case
    // theta is just the zenith angle of the track.
    // also please make sure angle_factor is positive. If its not Lev Landau will be sad.
    const double x = target.thickness/angle_factor;
    const double beta2 = beta*beta;
    const double gamma2 = 1/(1-beta2);
    double xi = x*(0.5*coeff*target.material.Z_A/avo)*charge*charge;
    double mpv = std::log(2*m_e*gamma2*xi/(target.material.I*target.material.I)) + 0.2 - beta2;
    xi /= beta2;
    mpv *= xi;
    return Landau{.location=mpv + 0.22278298*xi, .width=xi};
}

inline double log_likelihood(const double edep, const Landau& landau, const Detector& det){
    const double u = (edep-landau.location)/landau.width;
    const double v = landau.location/landau.width;
    return det.landau_kernel->Interpolate(u,v) - std::log(landau.width);
};

inline double log_likelihood(const std::vector<double>& edep, const std::vector<Landau>& landau, const Detector& det){
    double log_like=0;
    for(int i=0; i<edep.size(); ++i){
        log_like += log_likelihood(edep[i], landau[i], det);
    }
    return log_like;
};

}

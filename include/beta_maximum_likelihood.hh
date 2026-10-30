#pragma once

#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>

#include "detector.hh"
#include "landau_likelihood.hh"

namespace gaps_ml {

inline double most_likely_beta(
    const std::vector<double>& edep,
    const std::vector<double>& angf,
    const Detector det
){
    auto negative_log_likelihood = [&](const double* pars){
        const double beta = pars[0];
        double log_like = 0;
        for(int i=0; i<edep.size(); ++i){
            const auto landau = landau_pars(beta, angf[i], det);
            log_like += log_likelihood(edep[i], landau, det);
        }
        return -2*log_like;
    };

    auto minimizer = std::unique_ptr<ROOT::Math::Minimizer>(
        ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad")
    );
    ROOT::Math::Functor func(negative_log_likelihood, 1);
    minimizer->SetFunction(func);
    minimizer->SetLimitedVariable(0, "beta", 0.9, 0.01, 0.1, 0.999);
    minimizer->SetErrorDef(1.0);
    minimizer->Minimize();
    return minimizer->X()[0];
}

inline double most_likely_beta(
    const std::vector<double>& edep_sili,
    const std::vector<double>& angf_sili,
    const Detector sili,
    const std::vector<double>& edep_tof,
    const std::vector<double>& angf_tof,
    const Detector tof
){
    auto negative_log_likelihood = [&](const double* pars){
        const double beta = pars[0];
        double log_like = 0;
        for(int i=0; i<edep_sili.size(); ++i){
            const auto landau = landau_pars(beta, angf_sili[i], sili);
            log_like += log_likelihood(edep_sili[i], landau, sili);
        }
        for(int i=0; i<edep_tof.size(); ++i){
            const auto landau = landau_pars(beta, angf_tof[i], tof);
            log_like += log_likelihood(edep_tof[i], landau, tof);
        }
        return -2*log_like;
    };

    auto minimizer = std::unique_ptr<ROOT::Math::Minimizer>(
        ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad")
    );
    ROOT::Math::Functor func(negative_log_likelihood, 1);
    minimizer->SetFunction(func);
    minimizer->SetLimitedVariable(0, "beta", 0.9, 0.01, 0.1, 0.999);
    minimizer->SetErrorDef(1.0);
    minimizer->Minimize();
    return minimizer->X()[0];
}

}

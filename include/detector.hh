#pragma once

#include <memory>

#include "TH2.h"
#include <Math/Integrator.h>
#include <Math/PdfFuncMathCore.h>

namespace gaps_ml{

struct Material{
    double Z_A; // ratio of atomic charge to mass
    double I; // mean ionization energy
};

std::unique_ptr<TH2D> build_table(double energy_res);

struct Detector{
    Material material;
    double thickness;
    std::unique_ptr<TH2D> landau_kernel; // kernel of landau convovled with a gaussian
    Detector(Material mat, double w, double energy_res)
    : material(mat), thickness(w), landau_kernel(build_table(energy_res)){}
};

// this creates a look-up table for the "kernel" of a landau convovled with a gaussian
// it assumes that there is a constant fractional energy resolution "energy_res"
// it computes a "normalized" landau which only needs to be scaled for each velocity
inline std::unique_ptr<TH2D> build_table(double energy_res) {
    using ROOT::Math::IntegratorOneDim;
    using ROOT::Math::gaussian_pdf;
    using ROOT::Math::landau_pdf;

    constexpr double du = 0.5, dv = 0.5;
    constexpr double u_min = -5.0, u_max = 1500.0;
    constexpr double v_min =  4.0, v_max = 25.0;
    constexpr int n_u = (u_max - u_min)/du;
    constexpr int n_v = (v_max - v_min)/dv;

    auto table = std::make_unique<TH2D>("", ";u;v",
        n_u, u_min - du/2, u_max + du/2,
        n_v, v_min - dv/2, v_max + dv/2
    );
    table->SetDirectory(nullptr);

    IntegratorOneDim integrator;

    for(int i=1; i <= n_u; ++i){
        const double u = table->GetXaxis()->GetBinCenter(i);
        for (int j=1; j<=n_v; ++j){
            const double v = table->GetYaxis()->GetBinCenter(j);
            auto integrand = [=](double z){
                const double a = 1.0 + energy_res*z;
                if (a <= 0.0) return 0.0;
                return (gaussian_pdf(z)/a)*landau_pdf((u+v)/a, 1.0, v);
            };
            // idk 6.5-sigma is close to n_hits*n_events for GAPS flight one
            const double pdf = integrator.Integral(integrand, -6.5, 6.5);
            table->SetBinContent(i, j, std::log(pdf));
        }
    }
    return table;
}

constexpr double sili_wid = 0.23; // cm
constexpr double tof_wid = 0.635; // cm
constexpr double rho_si = 2.329085; // g/cm3
constexpr double rho_pvt = 1.023; // g/cm3

constexpr Material silicon{.Z_A=14./28.085, .I=173};
constexpr Material pvt{.Z_A=0.75779, .I=64.7};
}

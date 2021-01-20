#pragma once
#include "basis.hpp"
#include "pde.hpp"
#include "tensors.hpp"

template<typename P, typename PP>
void generate_all_coefficients(
    PDE<PP> &pde, basis::wavelet_transform<P, PP, resource::host> const &transformer,
    P const time = 0.0, bool const rotate = true);

template<typename P, typename PP>
fk::matrix<P> generate_coefficients(
    dimension<PP> const &dim, term<P> const &term_1D,
    partial_term<P> const &pterm,
    basis::wavelet_transform<P, PP, resource::host> const &transformer,
    P const time = 0.0, bool const rotate = true);

#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "program_options.hpp"
#include "quadrature.hpp"

#include "pde/pde_base.hpp"
#include "pde/pde_continuity1.hpp"
#include "pde/pde_continuity2.hpp"
#include "pde/pde_continuity3.hpp"
#include "pde/pde_continuity6.hpp"
#include "pde/pde_diffusion1.hpp"
#include "pde/pde_diffusion2.hpp"
#include "pde/pde_fokkerplanck1_4p3.hpp"
#include "pde/pde_fokkerplanck1_4p4.hpp"
#include "pde/pde_fokkerplanck1_4p5.hpp"
#include "pde/pde_fokkerplanck1_pitch_C.hpp"
#include "pde/pde_fokkerplanck1_pitch_E.hpp"
#include "pde/pde_fokkerplanck2_complete.hpp"
#include "tensors.hpp"

//
// this file contains the PDE factory and the utilities to
// select the PDEs being made available by the included
// implementations
//

// ---------------------------------------------------------------------------
//
// A free function factory for making pdes. eventually will want to change the
// return for some of these once we implement them...
//
// ---------------------------------------------------------------------------

template<typename P>
// the choices for supported PDE types
enum class PDE_opts
{
  continuity_1,
  continuity_2,
  continuity_3,
  continuity_6,
  fokkerplanck_1d_pitch_E,
  fokkerplanck_1d_pitch_C,
  fokkerplanck_1d_4p3,
  fokkerplanck_1d_4p4,
  fokkerplanck_1d_4p5,
  fokkerplanck_2d_complete,
  diffusion_1,
  diffusion_2,
  // FIXME will need to add the user supplied PDE choice
};

enum class PDE_case_opts
{
  mod0,
  mod1,
  mod2,
  mod_count
  // FIXME will need to add the user supplied PDE cases choice
};

std::unique_ptr<PDE<P>> make_PDE(parser const &cli_input)
{
  switch (cli_input.get_selected_pde())
  {
  case PDE_opts::continuity_1:
    return std::make_unique<PDE_continuity_1d<P>>(cli_input);
  case PDE_opts::continuity_2:
    return std::make_unique<PDE_continuity_2d<P>>(cli_input);
  case PDE_opts::continuity_3:
    return std::make_unique<PDE_continuity_3d<P>>(cli_input);
  case PDE_opts::continuity_6:
    return std::make_unique<PDE_continuity_6d<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_pitch_E:
    if(cli_input.get_pde_selected_case() == opts_case::mod1)
        return std::make_unique<PDE_fokkerplanck_1d_pitch_E<P,case_opts::mod1>>(cli_input);
    else
        return std::make_unique<PDE_fokkerplanck_1d_pitch_E<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_pitch_C:
    return std::make_unique<PDE_fokkerplanck_1d_pitch_C<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p3:
    return std::make_unique<PDE_fokkerplanck_1d_4p3<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p4:
    return std::make_unique<PDE_fokkerplanck_1d_4p4<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p5:
    return std::make_unique<PDE_fokkerplanck_1d_4p5<P>>(cli_input);
  case PDE_opts::fokkerplanck_2d_complete:
    return std::make_unique<PDE_fokkerplanck_2d_complete<P>>(cli_input);
  case PDE_opts::diffusion_1:
    return std::make_unique<PDE_diffusion_1d<P>>(cli_input);
  case PDE_opts::diffusion_2:
    return std::make_unique<PDE_diffusion_2d<P>>(cli_input);
  default:
    std::cout << "Invalid pde choice" << std::endl;
    exit(-1);
  }
}

// WARNING for tests only!
// features rely on options, parser, and PDE constructed w/ same arguments
// shim for easy PDE creation in tests
template<typename P>
std::unique_ptr<PDE<P>>
make_PDE(PDE_opts const pde_choice, fk::vector<int> levels,
         int const degree = parser::NO_USER_VALUE,
         double const cfl = parser::DEFAULT_CFL)
{
  return make_PDE<P>(parser(pde_choice, levels, degree, cfl));
}

// old tests based on uniform level need conversion
template<typename P>
std::unique_ptr<PDE<P>>
make_PDE(PDE_opts const pde_choice, int const level = parser::NO_USER_VALUE,
         int const degree = parser::NO_USER_VALUE,
         double const cfl = parser::DEFAULT_CFL)
{
  auto const levels = [level, pde_choice]() {
    if (level == parser::NO_USER_VALUE)
    {
      return fk::vector<int>();
    }

    switch (pde_choice)
    {
    case PDE_opts::continuity_1:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::continuity_2:
      return fk::vector<int>(std::vector<int>(2, level));

    case PDE_opts::continuity_3:
      return fk::vector<int>(std::vector<int>(3, level));

    case PDE_opts::continuity_6:
      return fk::vector<int>(std::vector<int>(6, level));

    case PDE_opts::fokkerplanck_1d_pitch_E:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::fokkerplanck_1d_pitch_C:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::fokkerplanck_1d_4p3:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::fokkerplanck_1d_4p4:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::fokkerplanck_1d_4p5:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::fokkerplanck_2d_complete:
      return fk::vector<int>(std::vector<int>(2, level));

    case PDE_opts::diffusion_1:
      return fk::vector<int>(std::vector<int>(1, level));

    case PDE_opts::diffusion_2:
      return fk::vector<int>(std::vector<int>(2, level));

    default:
      std::cout << "Invalid pde choice" << std::endl;
      exit(-1);
    }
  }();

  return make_PDE<P>(parser(pde_choice, levels, degree, cfl));
}

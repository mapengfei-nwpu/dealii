// ---------------------------------------------------------------------
//
// Copyright (C) 2003 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------



// check GeometryInfo::cell_to_child and back

#include <deal.II/base/geometry_info.h>

#include "../tests.h"


double
rand_2()
{
  return random_value<double>() * 4 - 2.;
}


template <int dim>
void
test()
{
  // Output normal directions for each face
  for (const unsigned int f : GeometryInfo<dim>::face_indices())
    {
      deallog << "Face " << f << ": n = ( ";
      for (unsigned int d = 0; d < dim; ++d)
        {
          if (d != 0)
            deallog << " , ";
          if (d == GeometryInfo<dim>::unit_normal_direction[f])
            deallog << GeometryInfo<dim>::unit_normal_orientation[f];
          else
            deallog << '0';
        }
      deallog << " )" << std::endl;
    }


  Point<dim> p;

  for (unsigned int ref_case_no = 1;
       ref_case_no <= RefinementPossibilities<dim>::isotropic_refinement;
       ++ref_case_no)
    {
      RefinementCase<dim> ref_case(ref_case_no);

      deallog << "RefinementCase=" << static_cast<unsigned int>(ref_case)
              << std::endl;
      // generate N random points in
      // [-2:2]^d, and transform them
      // back and forth between mother
      // and child cell
      const unsigned int N = 7;
      for (unsigned int i = 0; i < N; ++i)
        {
          for (unsigned int d = 0; d < dim; ++d)
            p[d] = rand_2();

          deallog << i << ' ' << p << ' '
                  << GeometryInfo<dim>::is_inside_unit_cell(p) << std::endl;
          for (unsigned int c = 0; c < GeometryInfo<dim>::n_children(ref_case);
               ++c)
            {
              const Point<dim> q =
                GeometryInfo<dim>::cell_to_child_coordinates(p, c);
              const Point<dim> pp =
                GeometryInfo<dim>::child_to_cell_coordinates(q, c);

              deallog << "    " << c << " [" << q << "] [" << pp << ']'
                      << std::endl;
              AssertThrow((p - pp).norm_square() < 1e-15 * 1e-15,
                          ExcInternalError());
              AssertThrow(GeometryInfo<dim>::is_inside_unit_cell(p) ==
                            GeometryInfo<dim>::is_inside_unit_cell(pp),
                          ExcInternalError());
            }
        }
    }
}


int
main()
{
  initlog();

  test<1>();
  test<2>();
  test<3>();

  return 0;
}

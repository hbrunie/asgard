#pragma once

template<typename P>
void stage_inputs_kronmult(P const *const x, P *const workspace,
                           int const num_elems, int const num_copies);

template<typename P>
void prepare_kronmult(int const *const flattened_table,
                      P *const *const operators, int const operator_lda,
                      P *const element_x, P *const element_work, P *const fx,
                      P **const operator_ptrs, P **const work_ptrs,
                      P **const input_ptrs, P **const output_ptrs,
                      int const degree, int const num_terms, int const num_dims,
                      int const elem_row_start, int const elem_row_stop,
                      int const elem_col_start, int const elem_col_stop);

template<typename P>
void call_kronmult(int const n, P *x_ptrs[], P *output_ptrs[], P *work_ptrs[],
                   P const *const operator_ptrs[], int const lda,
                   int const num_krons, int const num_dims);

void premix_convert_back(int64_t work_size, int64_t output_size,
                         double *dp_input, double *dp_output, double *dp_work,
                         float *sp_input, float *sp_output, float *sp_work);

void premix_convert(int64_t work_size, int64_t output_size, double *dp_input,
                    double *dp_output, double *dp_work, float *sp_input,
                    float *sp_output, float *sp_work);

function errCode = fgemm_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    m = ff_init_Size();
    k = ff_init_Size();
    A = ff_init_Matrix(F, m, k);
    n = ff_init_Size();
    B = ff_init_Matrix(F, k, n);
    beta = ff_init_Element(F);
    C = ff_init_Matrix(F, m, n);
    ref_res = modb(alpha* modb(A*B,F) + modb(beta*C,F) ,F);
    ff_res = fgemm(F, alpha, A, B, beta, C);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

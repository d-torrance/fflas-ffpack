function errCode = fsquare_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    n = ff_init_Size();
    A = ff_init_Matrix(F, n, n);
    beta = ff_init_Element(F);
    C = ff_init_Matrix(F, n, n);
    ref_res = modb(alpha*A*A+beta*C, F);
    ff_res = fsquare(F, alpha, A, beta, C);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

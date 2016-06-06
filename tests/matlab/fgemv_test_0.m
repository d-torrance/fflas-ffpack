function errCode = fgemv_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    m = ff_init_Size();
    n = ff_init_Size();
    A = ff_init_Matrix(F, m, n);
    X = ff_init_Vector(F, n);
    beta = ff_init_Element(F);
    Y = ff_init_Vector(F, m);
    ref_res = modb(alpha*A*X+beta*Y,F);
    ff_res = fgemv(F, alpha, A, X, beta, Y);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

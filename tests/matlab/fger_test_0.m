function errCode = fger_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    m = ff_init_Size();
    X = ff_init_Vector(F, m);
    n = ff_init_Size();
    Y = ff_init_Vector(F, n);
    A = ff_init_Matrix(F, m, n);
    ref_res = modb(alpha*X*Y'+A, F);
    ff_res = fger(F, alpha, X, Y, A);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

function errCode = faxpy_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    n = ff_init_Size();
    X = ff_init_Vector(F, n);
    Y = ff_init_Vector(F, n);
    ref_res = modb(alpha*X+Y,F);
    ff_res = faxpy(F, alpha, X, Y);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

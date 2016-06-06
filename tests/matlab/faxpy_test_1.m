function errCode = faxpy_test_1(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    alpha = ff_init_Element(F);
    m = ff_init_Size();
    n = ff_init_Size();
    A = ff_init_Matrix(F, m, n);
    B = ff_init_Matrix(F, m, n);
    ref_res = modb(alpha*A+B,F);
    ff_res = faxpy(F, alpha, A, B);
    eqTest = isequal(ff_res, ref_res);
    if ~eqTest
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

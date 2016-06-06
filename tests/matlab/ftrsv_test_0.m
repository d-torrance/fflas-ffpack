function errCode = ftrsv_test_0(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    n = ff_init_Size();
    X = ff_init_Vector(F,n);
    A = ff_init_Matrix(F,n,n);
    A = triu(A);
    ff_res = ftrsv(F, 'Upper', 'NonUnit', A, X);
    ref_res = modb( A*ff_res,F);
    eq = isequal(ref_res, X);
    if ~eq
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

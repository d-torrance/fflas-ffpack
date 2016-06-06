function errCode = ftrsv_test_2(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    n = ff_init_Size();
    X = ff_init_Vector(F,n);
    A = ff_init_Matrix(F,n,n);
    A = triu(A);
    A(1:n+1:n*n) = 1;
    ff_res = ftrsv(F, 'Upper', 'Unit', A, X);
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

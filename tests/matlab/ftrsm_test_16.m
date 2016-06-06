% ftrsm left - upper - size(A) == size(B) - alpha=1 - unit
function errCode = ftrsm_test_16(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    n = ff_init_Size();
    B = ff_init_Matrix(F,n,n);
    A = ff_init_Matrix(F,n,n);
    A = triu(A);
    A(1:n+1:n*n) = 1;
    ff_res = ftrsm(F, 'Left', 'Upper', 'Unit', 1, A, B);
    ref_res = modb( A*ff_res,F);

    eq = isequal(ref_res, B);
    if ~eq
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

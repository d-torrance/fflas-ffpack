% ftrmm left - upper - size(A) != size(B) B is not square matrix - alpha=1
function errCode = ftrmm_test_01(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    m = ff_init_Size();
    n = ff_init_Size();
    B = ff_init_Matrix(F,m,n);
    A = ff_init_Matrix(F,m,m);
    A = triu(A);
    ff_res = ftrmm(F, 'Left', 'Upper', 'NonUnit', 1, A, B);
    ref_res = modb( A*B,F);
    eq = isequal(ref_res, ff_res);
    if ~eq
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

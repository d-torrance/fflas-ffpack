% ftrsm left - lower - size(A) != size(B) B is not square matrix - alpha=rand
function errCode = ftrsm_test_13(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    m = ff_init_Size();
    n = ff_init_Size();
    alpha = ff_init_Element(F);
    B = ff_init_Matrix(F,m,n);
    A = ff_init_Matrix(F,m,m);
    A = tril(A);
    ff_res = ftrsm(F, 'Left', 'Lower', 'NonUnit', alpha, A, B);
    ref_res = modb( A*ff_res,F);

    eq = isequal(ref_res, modb(alpha*B, F));
    if ~eq
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

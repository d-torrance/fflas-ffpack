% ftrsm right - lower - size(A) != size(B) B is not square matrix -
% alpha=1 - unit
function errCode = ftrsm_test_18(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    m = ff_init_Size();
    n = ff_init_Size();
    alpha = ff_init_Element(F);
    B = ff_init_Matrix(F,m,n);
    A = ff_init_Matrix(F,n,n);
    A = tril(A);
    A(1:n+1:n*n) = 1;
    ff_res = ftrsm(F, 'Right', 'Lower', 'Unit', alpha, A, B);
    ref_res = modb( ff_res*A,F);

    eq = isequal(ref_res, modb(alpha*B, F));
    if ~eq
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

function errCode = fiszero_test_3(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    m = ff_init_Size();
    n = ff_init_Size();
    A = ff_init_Matrix(F,m,n);
    res = ~any(A);
    ff_res = fiszero(F, A);
    if ff_res ~= res
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

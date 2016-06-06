function errCode = fiszero_test_2(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    m = ff_init_Size();
    n = ff_init_Size();
    A = zeros(m,n);
    ff_res = fiszero(F, A);
    if ~ff_res
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

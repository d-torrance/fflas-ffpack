function errCode = fequal_test_1(p)
  errCode = 0;
  try
    addpath(p);
    ff_rand_init();
    F = ff_init_Field();
    n = ff_init_Size();
    X = ff_init_Vector(F,n);
    Y = X;
    ff_res = fequal(F, X, Y);
    res= X==Y;
    if ff_res ~= res
      error('Computation error');
    end
  catch exception;
    disp(exception.message);
    errCode = 1;
  end
end

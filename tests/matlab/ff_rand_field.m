function F=ff_rand_field(varargin)
    vmax = 2^20;
    if size(varargin) == 1
        vmax=varargin{1};
    end
    F=randi(vmax);
    while ~isprime(F)
        F=randi(vmax);
    end
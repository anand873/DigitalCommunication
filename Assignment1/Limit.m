function [F_out] = Limit(F_in,T,c,order)
    
    N = length(F_in);
    F_tl = zeros(size(F_in));
    for i = N/2 - T/2 + 1 : N/2 + T/2
        F_tl(i) = F_in(i);
    end
    F_out = lowpass(F_tl, c/(length(F_in)));
    figure
    plot([F_in F_out]);
    title(['Comparison of Input and Output (Order = ',num2str(order),') for c = ',num2str(c)]);
    legend('Original','Limited');
    axis([1 length(F_in) -0.15 0.15]);
    
end


function [Coeff] = Expnd(F,Seq)

    Coeff=zeros(5,1);
    
    for i=1:5
        Coeff(i) = dot(F,Seq(:,i));
    end
    
end


$$
\begin{align}
    [\text{Exec}] &\to [\text{Stmt}]^*
    \\
    [\text{Stmt}] &\to
    \begin{cases}
        \text{exit}([\text{Expr}]); \\
        \text{type} \space\text{indent} = [\text{Expr}];
    \end{cases}
    \\
    [\text{Expr}] &\to 
    \begin{cases}
        [\text{Term}]\\
        [\text{BinExpr}]
    \end{cases} \\
    [\text{BinExpr}] &\to
    \begin{cases}
        [\text{Expr}] + [\text{Expr}] & \text{prec} = 0 \\
        [\text{Expr}] - [\text{Expr}] & \text{prec} = 0 \\
        [\text{Expr}] * [\text{Expr}] & \text{prec} = 1 \\
        [\text{Expr}] / [\text{Expr}] & \text{prec} = 1 \\
    \end{cases} \\
    [\text{Term}] &\to 
    \begin{cases}
        \text{Integer\_Literal}\\
        \text{Indent} \\
        ([\text{Expr}]) \\
    \end{cases} \\
\end{align}
$$
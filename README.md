# House
---


The House function specifies the probability of reporting a specific phenotype \
at a specific time, conditioned on an $\epsilon$-neighborhood of a specific his\
tory:
\cgather[6pt]{
\forall t \in \T, d \in \D,\ \Psi_{\epsilon}(t,d,h_{t-1}) = Pr \left ( (t,d) \b\
ig \rvert \big [ h_{t-1} \big ]_{\epsilon}  \right )
}
where, denoting the chosen quasi-metric as $\Theta(\cdot,\cdot)$, we have:
\cgather{
h'_{t'} \in \big [ h_{t-1} \big ]_{\epsilon} \textrm{ if } \Theta(h'_{t'},h_t) \
< \epsilon
}
\end{defn}


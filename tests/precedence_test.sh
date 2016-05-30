#!/bin/bash

(echo a && echo b) && (echo b && echo c)

(echo z || echo b) && (echo c || echo d)

(echo a || echo b) && echo d

(echo a; echo b); echo c; echo d; ls

(echo a && echo b || echo c) && echo d

(asdfs || echo b) && echo b

(ls || echo b && echo c || echo d) && echo m

ls && (echo b && echo c)

ls; (echo b; echo c; || echo d && echo l)

(echo a || echo b) || (echo c || echo do)

(echo a && echo b) || (echo c && echo do)

(echo a && echo b && echo c && echo d)

(echo a || echo b || echo c || echo d)

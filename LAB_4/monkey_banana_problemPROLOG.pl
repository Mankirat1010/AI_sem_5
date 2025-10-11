% --------------------------------------------------
%   MONKEY AND BANANA PROBLEM  (Final Working Version)
% --------------------------------------------------

% state(MonkeyPos, MonkeyStatus, BoxPos, HasBanana)
% MonkeyStatus: onfloor / onbox
% HasBanana: has / hasnot

% --------------------------------------------------
% INITIAL AND GOAL STATES
% --------------------------------------------------

initial(state(atdoor, onfloor, atwindow, hasnot)).
goal(state(_, _, _, has)).

% --------------------------------------------------
% VALID LOCATIONS
% --------------------------------------------------

place(atdoor).
place(atwindow).
place(centerroom).

% --------------------------------------------------
% ACTIONS
% --------------------------------------------------

% 1. Walk
move(state(M, onfloor, B, H), walk(M, N), state(N, onfloor, B, H)) :-
    place(N), M \= N.

% 2. Push box
move(state(P, onfloor, P, hasnot), push(P, N), state(N, onfloor, N, hasnot)) :-
    place(N), P \= N.

% 3. Climb onto box
move(state(P, onfloor, P, hasnot), climb, state(P, onbox, P, hasnot)).

% 4. Grasp banana
move(state(centerroom, onbox, centerroom, hasnot), grasp,
     state(centerroom, onbox, centerroom, has)).

% --------------------------------------------------
% SEARCH (Depth-First with Visited-State Check)
% --------------------------------------------------

search(State, _, []) :-
    goal(State), !.

search(State, Visited, [Action|Rest]) :-
    move(State, Action, Next),
    \+ member(Next, Visited),
    search(Next, [Next|Visited], Rest).

% --------------------------------------------------
% MAIN SOLVER
% --------------------------------------------------

can_get_banana :-
    initial(State),
    search(State, [State], Plan),
    nl,
    write('==========================================='), nl,
    write('           MONKEY AND BANANA PLAN          '), nl,
    write('==========================================='), nl,
    forall(member(A, Plan), (write('Monkey performs: '), write(A), nl)),
    write('-------------------------------------------'), nl,
    write('Monkey has the banana!'), nl,
    write('==========================================='), nl, nl.

% --------------------------------------------------
% AUTO-RUN DIRECTIVE
% --------------------------------------------------
:- initialization((can_get_banana, halt)).

-module(tul1).
-export([covert/2]).

covert(N,inch) ->
	N / 2.54;
covert(M,centimeter) ->
	M * 2.54.

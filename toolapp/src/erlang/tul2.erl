-module(tul2).
-export([covert/1]).

covert({inch,X}) ->
	{centimeter,X / 2.54};

covert({centimeter,Y}) ->
	{inch,Y * 2.54}.

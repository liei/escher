%{
#include <stdio.h>
#include <parser.h>

#ifdef DUMP_TOKENS
    #define RETURN(t) do {                                          \
        fprintf ( stderr, "TOKN ( %d,\t'%s' )\n", t, yytext );     \
        return t;                                                   \
    } while ( 0 )
#else
    #define RETURN(t) return t
#endif

%}
%option array
%option noyywrap
%option yylineno

WHITESPACE [\ \t\n]
DIGIT      [0-9]
INTEGER    {DIGIT}+
DOUBLE     {DIGIT}+(.{DIGIT}+)?
COMMENT    \#[^\n]*\n

%%
{WHITESPACE}+    {}//ignore
{COMMENT}        {}//ignore

iterations       {RETURN(T_ITERATIONS);}
steps            {RETURN(T_STEPS);}
x_size           {RETURN(T_XSIZE);}
x_pos            {RETURN(T_XPOS);}
y_size           {RETURN(T_YSIZE);}
y_pos            {RETURN(T_YPOS);}
samples          {RETURN(T_SAMPLES);}
alpha            {RETURN(T_ALPHA);}
beta             {RETURN(T_BETA);}

{INTEGER}        {RETURN(INTEGER);}
{DOUBLE}         {RETURN(DOUBLE);}
.                {RETURN((int) yytext[0]);}

%%
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <simulation.h>


extern char yytext[];
extern int yylineno;
extern FILE *yyin;

int yyparse(void);
int yylex(void);
void yyerror(const char *str);

static simulation_t *yysim;

/* linked list implementation to parse the alpha/beta probs */
typedef struct l{
  double data;
  struct l *next;
  int length;
} list_node;

void assign_list(int token,list_node *node);
void parse_sim(simulation_t *sim, char *filename);

%}

%token T_ITERATIONS T_STEPS T_SAMPLES T_X T_XSIZE T_XPOS T_Y T_YSIZE T_YPOS T_ALPHA T_BETA 

%union {
  int t_integer;
  double t_double;
  struct l  * t_node;
}

%token <t_integer> INTEGER
%token <t_double> DOUBLE

%type <t_node> list node
%type <t_integer> integer 

%%

assignment_list:    /*empty*/
               |    assignment_list assignment
               ;

assignment:         T_ITERATIONS  '=' integer       {yysim->iterations = $3;}
          |         T_STEPS '=' integer             {yysim->steps = $3;}
          |         T_SAMPLES '=' integer           {yysim->samples = $3;}
          |         T_X '=' integer '/' integer     {yysim->x_pos = $3; yysim->x_size = $5;}
          |         T_XSIZE '=' integer             {yysim->x_size = $3;}
          |         T_XPOS  '=' integer             {yysim->x_pos = $3;}
          |         T_Y '=' integer '/' integer     {yysim->y_pos = $3; yysim->y_size = $5;}
          |         T_YSIZE '=' integer             {yysim->y_size = $3;}
          |         T_YPOS  '=' integer             {yysim->y_pos = $3;}
          |         T_ALPHA '=' '[' list ']'        {assign_list(T_ALPHA,$4);}
          |         T_BETA '=' '[' list ']'         {assign_list(T_BETA,$4);}          
          ;

list:               node                            {$$ = $1;}
    |               node ',' list                   {$$ = $1;$$->next = $3;$$->length = $3->length + 1;}
    ;

node:             DOUBLE                            {$$ = malloc(sizeof(list_node));*$$ = (list_node){atof(yytext),NULL,1};}
    ;

integer:            INTEGER                         {$$ = atoi(yytext);}
       ;

%%

void yyerror(const char *str){
  fprintf(stderr,"error:%d - %s \n",yylineno,str);
}

void assign_list(int token,list_node *node){
  yysim->steps = node->length;
  double *a = malloc(sizeof(double) * node->length);
  list_node *old;
  for(int i = 0;node != NULL; i++){
    a[i] = node->data;
    node = (old = node)->next;
    free(old);
  }
  switch(token){
  case T_ALPHA: yysim->alpha = a; break;
  case T_BETA: yysim->beta = a; break;
  }
}

void parse_sim(simulation_t *sim, char *filename){
  yysim = sim;
  if((yyin = fopen(filename,"r")) == NULL){
    fprintf(stderr,"Couldn't read simulation file: %s.\n",filename);
    exit(EXIT_FAILURE);
  }
  yyparse();
}

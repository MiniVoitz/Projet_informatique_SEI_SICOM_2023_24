//fichier pour la fonction list_t re_read(char *re)

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> /* NULL */
#include <string.h>

#include "pyas/all.h"

binaryblock_t fonction_crochet(char* expr)
{
	binaryblock_t t = calloc(1,sizeof(*t));


	if(NULL == expr)
	{
		printf("Error: Nothing in the [ ] \n");
		t->array = NULL;
		t->crochet = 1;
		t->operateur = 'R';
		t->negation = 0;
		t->err = 1;
		return t;
	}

	if (strcmp(expr, "") == 0)
	 {
		 printf("Error: Nothing in the [ ] \n");
		t->array = NULL;
		t->crochet = 1;
		t->operateur = 'R';
		t->negation = 0;
		t->err = 1;
		return t;
	}

	//Some very specific case
	int *tbl = calloc(128,sizeof(int));





	int tab[128];
	int i;
	for (i = 0; i < 128; i++)
	{
		tab[i] = 1;
	}

  // Set specific characters to 0
  tab['+'] = 0;
	tab['-'] = 0;
  tab['*'] = 0;
  tab['?'] = 0;
  tab['.'] = 0;
  tab['^'] = 0;
	tab['['] = 0;
	tab[']'] = 0;
	tab['\\'] = 0;



	char * p1 = NULL;
	int len = strlen(expr);

	for(p1=expr; p1 < (expr+len) ;p1++) //looking for the char into []
	{


		if(tab[(int)(*p1)] == 1)  // the char isn't special
		{

				tbl[(int)(*p1)] = 1;
		}

		else if (*p1 == '^')
		{
			printf("Erreur syntaxe pas de négation dans les crochets \n");
			t->array = tbl;
			t->crochet = 1;
			t->operateur = 'R';
			t->negation = 0;
			t->err = 1;
			return t;
		}
		else if (*p1 == '[') //Crochet de crochet ?
		{
			printf("Error : braket into braket \n");
			t->array = tbl;
			t->crochet = 1;
			t->operateur = 'R';
			t->negation = 0;
			t->err = 1;
			return t;
		}

		else if (*p1 == '\\')
		{
			p1++;
			switch (*(p1))
			{
					case 'n':
					tbl[10] = 1;
					break;

					case 't':
					tbl[9] = 1;
					break;

					case '+':
					tbl[(int)(*p1)] = 1;
					break;

					case '-':
					tbl[(int)(*p1)] = 1;
					break;

					case '\\':
					tbl[(int)(*p1)] = 1;
					break;

					case '*':
					tbl[(int)(*p1)] = 1;
					break;

					case '?':
					tbl[(int)(*p1)] = 1;
					break;

					case '[':
					tbl[(int)(*p1)] = 1;
					break;

					case ']':
					tbl[(int)(*p1)] = 1;
					break;

					case '^':
					tbl[(int)(*p1)] = 1;
					break;

					default:

					printf("Error: '\\' can't be followed by %c \n",*p1);
					free(tbl);
					t->array = NULL;
					t->crochet = 1;
					t->operateur = 'R';
					t->negation = 0;
					t->err = 1;
					return t;
					break;

			}
		}


		else if (*p1 == '-')
		{
			if(*expr == '-') // Error => Start with -
			{
				printf("Error: '-' can't be the first character \n");
				free(tbl);
				t->array = NULL;
				t->crochet = 1;
				t->operateur = 'R';
				t->negation = 0;
				t->err = 1;
				return t;
				return t;
			}

			else if(*(p1+1) == '\0') //expr + len == p1 ?
			{
				printf("Error: '-' can't be the last character in [ ] \n");
				free(tbl);
				t->array = NULL;
				t->crochet = 1;
				t->operateur = 'R';
				t->negation = 0;
				t->err = 1;
				return t;
				return t;
			}

			else
			{
				int prev = (int)(*(p1-1));
				int next = (int)(*(p1+1));
				if(*(p1+2)=='-')
				{printf("- argument not accepted\n");
				free(tbl);
				t->array = NULL;
				t->crochet = 1;
				t->operateur = 'R';
				t->negation = 0;
				t->err = 1;
				return t;}
				if((47<prev && prev<58 && 47<next && next<58)||(64<prev && prev<91 && 64<next && next<91)||(96<prev && prev<123 && 96<next && next<123)) // Verification of intervale correctness
				{
					int min = prev < next ? prev : next;
					int max = prev > next ? prev : next;
					int k;
					for(k=min;k<max;k++)
					{
						tbl[k] = 1;
					}
				}
				else
				{
					printf("Error : Wrong intervale\n");
					free(tbl);
					t->array = NULL;
					t->crochet = 1;
					t->operateur = 'R';
					t->negation = 0;
					t->err = 1;
					return t;
					return t;
				}
			}
		}

		else
		{
			printf("Error: '%c' can't be in [ ]\n",*expr);
			free(tbl);
			t->array = NULL;
			t->crochet = 1;
			t->operateur = 'R';
			t->negation = 0;
			t->err = 1;
			return t;
				return t;
		}
	}

	t->array = tbl;
	t->crochet = 1;
	t->operateur = 'R';
	t->negation = 0;
	t->err = 0;
	return t;
}



list_t re_read(char *regexp) //if null exit(Error)
{
//Some very specific case
	if(NULL == regexp)
	{
		return NULL;
	}

	if (strcmp(regexp, "") == 0)
	 {
			return NULL;
		}
	//--------------------------------------------------------------
		//Creation of the queue which is going to be converted as list the return at the end of function

	queue_t q = queue_new();

	//---------------------------------------------------------------
	int tab[128];
	int i;
	for (i = 0; i < 128; i++)
	{
		tab[i] = 1;
	}

  // Set specific characters to 0
  tab['+'] = 0;
  tab['*'] = 0;
  tab['?'] = 0;
  tab['.'] = 0;
  tab['^'] = 0;
	tab['['] = 0;
	tab[']'] = 0;
	tab['\\'] = 0;

	char * p = NULL;
	int negation = 0; //Boolean 1 if we find a ^
	int len = strlen(regexp);
	list_t l = list_new();
	for(p = regexp; p < regexp + len ; p++) //cas d'arret ?
	{
		if(tab[(int)(*p)] == 1)  // 1rst character of re is within a-z A-Z or 0-9 then the if is validated
		{
				binaryblock_t t = calloc(1,sizeof(*t));
				int *tbl = calloc(128, sizeof(int));

				tbl[(int)(*p)] = 1;

				t->array = tbl;

				t->operateur = 'R';

				t->negation = negation;
				if(t->negation)
				{
					int n;
					for(n=0;n<128;n++)
					{
						(t->array)[n] = (((t->array)[n])+1)%2;
					}
				}
				negation = 0;
				t->err = 0;
				q = enqueue(q,t);
		}

		else if (*p == '.')
		{
			binaryblock_t t = calloc(1,sizeof(*t));
			int *tbl = calloc(128, sizeof(int));
			t->array = tbl;
			t->operateur = 'R';
			t->point = 1;
			t->negation = 0;
			if(negation)
			{
				printf("Error: '^.' can't happen \n");
				if(queue_empty(q))
				{
					t->err = 1;
					q = enqueue(q,t);
				}
				else
				{
					((binaryblock_t)(q->next->content))->err = 1;
				}
				l = queue_to_list(q);
				return l;
			}
			q = enqueue(q,t);
		}


		else if (*p == '^')
		{
			if(p == regexp + len - 1)
			{
				printf(" '^' Can't be at the end of the regular expression \n");
				if(queue_empty(q))
				{
					binaryblock_t t = calloc(1,sizeof(*t));
					int *tbl = calloc(128, sizeof(int));
					t->array = tbl;
					t->crochet = 1;
					t->operateur = 'R';
					t->negation = 0;
					t->err = 1;
					q = enqueue(q,t);
				}
				else
				{
					((binaryblock_t)((q->next)->content))->err = 1;
				}
				l = queue_to_list(q);
				return l;
			}
			negation = 1;
		}



		else if (*p == '[')
		{
			int *tbl = calloc(128, sizeof(int));
			int i = 1;
			char* e = calloc(len,sizeof(char)); // Dynamic memory alloc for coping the inside of the [ ]
			p++;//jump [

			while((*(p) != ']') && (i<(len))) // *p = [ => start at p+1
			{
					if(*(p) == '[')
					{
						printf("Error [ into []");
						free(e);
						if(queue_empty(q))
						{
							binaryblock_t t = calloc(1,sizeof(*t));
							t->array = NULL;
							t->crochet = 1;
							t->operateur = 'R';
							t->negation = 0;
							t->err = 1;
							q = enqueue(q,t);
						}
						else
						{
							((binaryblock_t)(q->next->content))->err = 1;
						}
						free(tbl);
						l = queue_to_list(q);
						return l;
					}
					*(e+i-1) = *(p);
					p++;
					i++;
			}

			if((len) == i)
			{
				free(e);
				printf("Error ] not found \n");
				if(queue_empty(q))
				{
					binaryblock_t t = calloc(1,sizeof(*t));
					t->array = tbl;
					t->crochet = 1;
					t->operateur = 'R';
					t->negation = 0;
					t->err = 1;
					q = enqueue(q,t);
				}
				else
				{
					((binaryblock_t)(q->next->content))->err = 1;
				}
				l = queue_to_list(q);
				return l;
			}

			else
			{
				binaryblock_t t = fonction_crochet(e);
				if(t->err == 1)
				{
					if(queue_empty(q))
					{
						if(t->array)
						{
							free(t->array);
						}
						t->array = NULL;
						t->crochet = 1;
						t->operateur = 'R';
						t->negation = 0;
						t->err = 1;
						q = enqueue(q,t);
					}
					else
					{
						del_binary(t);
						((binaryblock_t)(q->next->content))->err = 1;
					}
					free(tbl);
					free(e);
					l = queue_to_list(q);
					return l;
				}
				t->negation = negation;
				if(t->negation)
				{
					int n;
					for(n=0;n<128;n++)
					{
						(t->array)[n] = (((t->array)[n])+1)%2;
					}
				}
				free(tbl);
				negation = 0;
				q = enqueue(q,t);
				free(e);
			}
		}

		else if((*p == '+')||(*p == '*')||(*p == '?')) //Postfixe
		{

			if(queue_empty(q))
			{
				printf("Error: %c can't be the first character \n", *p);
				{
					binaryblock_t t = calloc(1,sizeof(*t));
					int *tbl = calloc(128, sizeof(int));
					t->array = tbl;
					t->crochet = 1;
					t->operateur = 'R';
					t->negation = 0;
					t->err = 1;
					q = enqueue(q,t);
					l = queue_to_list(q);
				return l;
				}
			}
			else
			{
			 	if((((binaryblock_t)(q->content))->operateur )!= 'R') //We look at the last in queue to see if there is any operator applied
					{
			 		printf("Error: two operator in a row");
			 		printf("First : %c Second : %c \n",((binaryblock_t)(q->content))->operateur,*p);
					((binaryblock_t)(q->next->content))->err = 1;
					l = queue_to_list(q);
					return l;
			 		}
			 	else
			 		{
					((binaryblock_t)(q->content))->operateur = *p;
			 		}
			}
		}


		else if (*p == '\\')
		{
			binaryblock_t t = NULL;
			t = calloc(1,sizeof(*t));
			int *tbl = calloc(128,sizeof(int));
			p++;
			switch (*(p))
			{

					case '\0':
					printf("Error : \\ can't be at the end\n");
					if(queue_empty(q))
					{
						t->array = NULL;
						t->crochet = 1;
						t->operateur = 'R';
						t->negation = 0;
						t->err = 1;
						q = enqueue(q,t);
					}
					else
					{
						((binaryblock_t)(q->next->content))->err = 1;
						del_binary(t);
					}
					free(tbl);
					l = queue_to_list(q);
					return l;
					break;

					case 'n':
					tbl[10] = 1;
					break;

					case 't':
					tbl[9] = 1;
					break;

					case '+':
					tbl[(int)(*p)] = 1;
					break;

					case '-':
					tbl[(int)(*p)] = 1;
					break;

					case '\\':
					tbl[(int)(*p)] = 1;
					break;


					case '*':
					tbl[(int)(*p)] = 1;
					break;


					case '?':
					tbl[(int)(*p)] = 1;
					break;

					case '.':
					tbl[(int)(*p)] = 1;
					break;

					case '#':
					tbl[(int)(*p)] = 1;
					break;

					case '[':
					tbl[(int)(*p)] = 1;
					break;

					case ']':
					tbl[(int)(*p)] = 1;
					break;

					case '^':
					tbl[(int)(*p)] = 1;
					break;


					default:
					printf("Error: \\ can't be followed by '%c'  \n", *p );
					if(queue_empty(q))
					{
						t->array = NULL;
						t->crochet = 1;
						t->operateur = 'R';
						t->negation = 0;
						t->err = 1;
						q = enqueue(q,t);
					}
					else
					{
						del_binary(t);
						((binaryblock_t)(q->next->content))->err = 1;
					}

					free(tbl);
					l = queue_to_list(q);
					return l;
					break;

				}
			t->array = tbl;
			t->operateur = 'R';
			t->negation = negation;
			if(t->negation)
			{
				int n;
				for(n=0;n<128;n++)
				{
					(t->array)[n] = (((t->array)[n])+1)%2;
				}
			}
			negation = 0;
			t->err = 0;
			q = enqueue(q,t);
			}





		else
		{
			printf("Erreur sur le caractère : '%c' il n'est pas sensé existé \n", *p);
			if(queue_empty(q))
			{
				binaryblock_t t = calloc(1,sizeof(*t));
				int *tbl = calloc(128, sizeof(int));
				t->array = tbl;
				t->crochet = 1;
				t->operateur = 'R';
				t->negation = 0;
				t->err = 1;
				q = enqueue(q,t);
			}
			else
			{
				((binaryblock_t)(q->next->content))->err = 1;
			}
			l = queue_to_list(q);
			return l;
		}
	}

	return queue_to_list(q);
	}

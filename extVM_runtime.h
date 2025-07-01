#ifndef EXTVM_RUNTIME
#define EXTVM_RUNTIME 1

#include "extVM_base.h"
#include "extVM_OC.h"
#include "extVM_answers.h"



INTI extVM_run(CT*ct_strt,CT* ct_prev_,BYTE max_regi)
	{
		INTI ans=ANSWER_ALL_IS_FINE;
		CT *ct_th,//actual CT chain
			*ct_tmp,//temporary CT chain
			*ct_prev;
		BS *bs_head,*bs_tmp;
		BYTE b,//byte of command
			 regi_p,*regi;
		bs_head = NULL;
		ct_th = ct_strt;
		ct_prev = ct_prev_;
		if(max_regi)
		regi = malloc(sizeof(BYTE)*(((INTI)max_regi)+1));
		else regi = NULL;
		regi_p = 0;
		while(1)
			{	
				if(ans)
					b = _HALT;
				else
				if(ct_th)
					{
					b = ct_th->b;	
					}
				else
					{
					ans = ANSWER_CT_ERROR;	
					continue;
					}
				if(b!=_NOP)
				if(b==_HALT)
					{
						while(bs_head)
							{
								
								bs_tmp=bs_head->x;
								BS_del(bs_head);
								bs_head = bs_tmp;
							}
						if(regi) free(regi);
						break;
					}
				else
				if(b==_CHNG)
					{
						ct_tmp = ct_th->x;
						CT_next(ct_tmp,ct_prev,ct_tmp);
						ct_prev = ct_tmp;
					}
				else
				if(b==_JMP)
					{
						if(bs_head)
							{
								b = bs_head->b;
								do
								{
									
									if(!ct_th)
										{
											ans = ANSWER_CT_ERROR;
											break;
										}
									b=b-1;
									ct_tmp = ct_th->x;
									CT_next(ct_tmp,ct_prev,ct_tmp);
									ct_prev = ct_th;
									ct_th =ct_tmp;
								}
								while(b);
								
							}
						else
							{
								ans = ANSWER_BS_ERROR;
								//continue;
							}
						
					}
				else
				if(b==_JMPZ)
					{
						if(bs_head)
							{
								bs_tmp = bs_head;
								while(bs_tmp)
									{
										if(!bs_tmp) break;
										b = bs_tmp->b;
										do
											{
												if(!ct_th)
													{
													ans = ANSWER_CT_ERROR;
													break;	
													}
												b = b-1;
												ct_tmp = ct_th->x;
												CT_next(ct_tmp,ct_prev,ct_tmp);
												ct_prev = ct_th;
												ct_th =ct_tmp;
											}
										while(b);
										
										if(ans) break;
										else
										if(bs_tmp->b) break;
										else  bs_tmp=bs_tmp->x;
									}
								
							}
						else
							{
								ans = ANSWER_BS_ERROR;
								//continue;
							}
						
					}
				else
				if(b==_GOTO)
					{
						b = ANSWER_BS_ERROR;
						if(bs_head)
							if(bs_head->x)
								if(bs_head->x->x)
									{
										if(bs_head->b)
											b = bs_head->x->x->b;
											
										else
											b = bs_head->x->b;
										do
											{
												if(!ct_th)
													{
													b = ANSWER_CT_ERROR;
													break;	
													}
												b = b-1;
												ct_tmp = ct_th->x;
												CT_next(ct_tmp,ct_prev,ct_tmp);
												ct_prev = ct_th;
												ct_th =ct_tmp;
												
											}
										while(b);
										
									}
						ans = b;
								
					}
				else
				switch(b)
					{
						case _PUSH:
							{
								ct_tmp = ct_th->x;
								CT_next(ct_tmp,ct_prev,ct_tmp);
								if(ct_tmp)
									{
										ct_prev = ct_th;
										ct_th =ct_tmp;
										BS_new(bs_tmp);
										bs_tmp->x = bs_head;
										bs_head = bs_tmp;
										bs_head->b = ct_th->b;
									}
								else
									ans = ANSWER_CT_ERROR;
								break;
							}
						case _POP:
							{
								if(bs_head)
									{
										bs_tmp=bs_head;
										bs_head = bs_head->x;
										BS_del(bs_tmp);
									}
								else
									ans = ANSWER_BS_ERROR;
								break;
							}
						case _LET:
							{
								if(bs_head)
									regi_p = bs_head->b;
								else
									ans = ANSWER_BS_ERROR;
								break;
							}
						case _PUT:
							{
								BS_new(bs_tmp);
								bs_tmp->x = bs_head;
								bs_head = bs_tmp;
								bs_head->b=regi_p;
								break;
							}
						case _GET:
							{
								if(regi_p<=max_regi)
									{
										BS_new(bs_tmp);
										bs_tmp->x = bs_head;
										bs_head = bs_tmp;
										bs_head->b = regi[regi_p];
										//bs_head->b = *(regi+regi_p);
									}
								else
									ans = ANSWER_RG_ERROR;
								break;
							}
						case _SET:
							{
								if(regi_p>max_regi)
									ans = ANSWER_RG_ERROR;
								else
								if(bs_head)
									{
									regi[regi_p] = bs_head->b;
									//*(regi+regi_p) = bs_head->b;
									}
								else
									ans = ANSWER_BS_ERROR;
								break;
							}
						
						case _COPY:
							{
								if(bs_head)
									{
										BS_new(bs_tmp);
										bs_tmp->x=bs_head;
										bs_tmp->b=bs_head->b;
										bs_head = bs_tmp;
									}
								else
									ans = ANSWER_BS_ERROR;
								break;
							}
							
						case _SWAP:
							{
								ans = ANSWER_BS_ERROR;
								if(bs_head)
									if(bs_head->x)
										{
											ans = ANSWER_ALL_IS_FINE;
											b = bs_head->b;
											bs_head->b = bs_head->x->b;
											bs_head->x->b = b;
										}
								break;
							}
						
						case _ZERO:
							{
								BS_new(bs_tmp);
								bs_tmp->x=bs_head;
								bs_head = bs_tmp;
								bs_head->b=0;
								break;
							}
							
						case _ACCS:
							{
								if(bs_head)
									{
										b = regi_p;
										bs_tmp=bs_head;
										while(1)
											if(b)
												if(bs_tmp->x)
													{
														bs_tmp=bs_tmp->x;
														b=b-1;
													}
												else
													{
														ans=ANSWER_BS_ERROR;
														break;
													}
											else
												{
													b = bs_tmp->b;
													BS_new(bs_tmp);
													bs_tmp->x=bs_head;
													bs_head=bs_tmp;
													bs_head->b=b;
													break;
												}
												
											
									}
								else ans = ANSWER_BS_ERROR;
								
								break;
							}
							
						case _NOT:
							{
								if(bs_head)
									{
										b = bs_head->b;
										b = (~b);
										BS_new(bs_tmp);
										bs_tmp->x=bs_head;
										bs_head=bs_tmp;
										bs_head->b=b;
									}
								else ans = ANSWER_BS_ERROR;
								break;
							}
							
						case _AND:
							{
								ans = ANSWER_BS_ERROR;
								if(bs_head)
									if(bs_head->x)
										{
											ans = ANSWER_ALL_IS_FINE;
											BS_new(bs_tmp);
											b = ((bs_head->b)&(bs_head->x->b));
											bs_tmp->x=bs_head;
											bs_head=bs_tmp;
											bs_head->b=b;
										}
								break;		
							}
						case _LSHFT:
							{
								if(bs_head)
									{
										b = bs_head->b;
										b = (b<<1);
										BS_new(bs_tmp);
										bs_tmp->x=bs_head;
										bs_head=bs_tmp;
										bs_head->b=b;
									}
								else ans = ANSWER_BS_ERROR;
								break;
							}
						
						case _RSHFT:
							{
								if(bs_head)
									{
										b = bs_head->b;
										b = (b>>1);
										BS_new(bs_tmp);
										bs_tmp->x=bs_head;
										bs_head=bs_tmp;
										bs_head->b=b;
									}
								else ans = ANSWER_BS_ERROR;
								break;
							}
						
						case 100:
							{
								printf("%u\n",bs_head->b);
								break;
							}
						
						
					}
				
				if(!ans)
					{
					ct_tmp = ct_th->x;
					CT_next(ct_tmp,ct_prev,ct_tmp);
					ct_prev = ct_th;
					ct_th = ct_tmp;
					}
				
				
			}
		
		
		return ans;
	}



#endif
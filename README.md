# ASM
1.0 Регистры процессора

ax, bx, cx, dx, flag, ip

1.1 Синтаксис

  а) Каждая комманда с ее аргуметами стоит в отдельной строке
  
  б) Каждая метка стоит в отдельной строке, заканчивающейся двоеточием. В имени метки не может быть пробелов, перед двоеточием тоже

  Хорошие метки
  
  label666:
  
  again:
  
  du_hast:
  
  
  Плохие
  
  vechnost pahnet neftyu:
  
  Пробелы в имени (Undeclared 'vechnost') 
  
  label
  
  нет двоеточия (Undeclared 'label')
  
  qqq :
  
  пробел перед : (Undeclared 'qqq')
  
  :
  
  компиляция пройдет успешно, но вы не сможете к такой метке обратиться(попытка написать jmp приведет к too few arguments)
  
  label1: eqweqwe
  
  строка закончилась не двоеточием (Wrong syntax in label definition 'eqweqwe')
  
  
  
  в) Комментарии не реализованы:( (coming soon)
  
1.2 Комманды процессора


COMMAND(mov, 1, 1, 0)    mov %регистр% %число%      // %регистр% = %число%

COMMAND(movr, 2, 0, 0)   mov %регистр% %регистр%    //%регистр% = %число%


COMMAND(push, 0, 1, 0)   push %число%               //положить число в стек

COMMAND(pushr, 1, 0, 0)  pushr %регистр%            //положить значение регистра в стек

COMMAND(pop, 1, 0, 0)    pop %регистр%              //вытолкнуть число из стека в регистр(


COMMAND(adds, 0, 0, 0)   adds                       //сложить два верхних чила из стека, результат записать в стек

COMMAND(subs, 0, 0, 0)   subs                       //вычесть ----//-----

COMMAND(muls, 0, 0, 0)   muls                       //умножить -----//-----

COMMAND(divs, 0, 0, 0)   divs                       //поделить ----//----

COMMAND(addr, 2, 0, 0)   addr %регистр1% %регистр2% // %регистр1% += %регистр2%

COMMAND(subr, 2, 0, 0)   subr %регистр1% %регистр2% // -=

COMMAND(mulr, 2, 0, 0)   mulr %регистр1% %регистр2% // *=

COMMAND(divr, 2, 0, 0)   divr %регистр1% %регистр2% // /=


COMMAND(addn, 1, 1, 0)   addn %регистр% %число%     //%регистр% += %число%

COMMAND(subn, 1, 1, 0)   subn %регистр% %число%     // -=

COMMAND(muln, 1, 1, 0)   muln %регистр% %число%     // *=

COMMAND(divn, 1, 1, 0)   divn %регистр% %число%     // /=


COMMAND(inr, 1, 0, 0)    inr %регистр%              // считать значение и положить в регистр %регистр%

COMMAND(outr, 1, 0, 0)   outr %регистр%             // вывести значение регистра %регистр%


COMMAND(cmpr, 2, 0, 0)   cmpr %регистр1% %регистр2% // сравнить знчаения в регистрах %регистр1% %регистр2%, если первое больше, регистру flag присваивается 1, -1 если меньше, 0 если равны


COMMAND(jmp, 0, 0, 1)    jmpr %метка%               //прыгнуть на метку %метка%

COMMAND(ja, 0, 0, 1)     ja %метка%                 // прыгнуть на метку метка %метка%, если flag > 0

COMMAND(jae, 0, 0, 1)    jae %метка%                //                                     flag >= 0

COMMAND(jb, 0, 0, 1)     jb %метка%                 //                                       flag < 0

COMMAND(jbe, 0, 0, 1)    jbe %метка%                //                                     flag <= 0

COMMAND(je, 0, 0, 1)     je %метка%                 //                                       flag == 0

COMMAND(jne, 0, 0, 1)    jne %метка%                //                                     flag != 0


COMMAND(hlt, 0, 0, 0)    hlt                        // завершить процесс


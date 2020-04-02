% 10 - Prática de Engenharia Reversa
% Sistemas Hardware-Software - 2020/1
% Igor Montagner

As aulas de hoje e quinta serão dedicadas a revisar conceitos básicos vistos nas últimas 5 aulas. A parte 1 repassa ponteiros e variáveis globais.  A parte 2 contém exercícios intermediários que misturam dois conceitos vistos em aula (ex: loops e `LEA` ou funções + condicionais). Está indicado ao lado de cada exercício quais conceitos são exercitados e todos eles serão recebidos via repositório de atividades da disciplina.

**Atenção**: acesse a aba "Repositório de Atividades" no Teams para indicar seu usuário do blackboard e endereço do repositório privado no Github.

Todo exercício para entrega deverá ser colocado no repositório de atividades da disciplina.

# Parte 1 - Ponteiros e variáveis globais

Todos os exercícios da revisão serão feitos com o arquivo `exemplo1` (compilado a partir de `exemplo1.c`).

## Ponteiros

```asm
Dump of assembler code for function muda_valor:
   0x05fa <+0>:	    addl   $0x1,0x200a0f(%rip)        # 0x201010 <var_global>
   0x0601 <+7>:	    lea    (%rsi,%rsi,2),%eax
   0x0604 <+10>:	add    %eax,%edi
   0x0606 <+12>:	mov    %edi,(%rdx)
   0x0608 <+14>:	lea    (%rdi,%rdi,2),%eax
   0x060b <+17>:	add    %eax,0x2009ff(%rip)        # 0x201010 <var_global>
   0x0611 <+23>:	retq
```

Nas linhas `+7` até `+12` podemos ver claramente a diferença entre a instrução `LEA` e `MOV`

* linha `+7`: é colocado no **registrador** `%eax` o **valor** `%rsi + 2*%rsi`.
* linha `+12`: é colocado na **memória**, no endereço gravado em `%rdx`, o valor do registrador **%edi** (4 bytes)

Ou seja, um `MOV` que se utiliza de parênteses represente o operador *variável apontada por* (**\***) em *C*. Um `LEA` nunca acessa a memória.

**Exercício**: Levando as informações acima em conta, faça a tradução das linhas `+7` até `+14` de `muda_valor`

```asm
   0x0601 <+7>:	   eax = 3*rsi  
   0x0606 <+12>:	   *rdx = edi
```

Vamos agora analisar as linhas `+0` e `+17`: 

```asm
   0x05fa <+0>:	    addl   $0x1,0x200a0f(%rip)        # 0x201010 <var_global>
   0x0601 <+7>:	    lea    (%rsi,%rsi,2),%eax

   0x060b <+17>:	add    %eax,0x2009ff(%rip)        # 0x201010 <var_global>
   0x0611 <+23>:	retq   
```

O parênteses indica que estamos mexendo na memória e o fato de estarmos usando o registrador `%rip` indica que os dados apontados são globais. Ou seja, eles tem visibilidade no programa todo e existem durante toda a execução do programa. Este cálculo é feito usando deslocamentos relativos ao endereço da instrução atual. Vejamos um exemplo no caso da linha `+0`.

1. Quando a CPU executa a linha `+0` o registrador `%rip` aponta para a linha seguinte (`0x0601`).
2. O resultado do lado direito do `addl` pede acesso a memória na posição `%rip + 0x200a0f` 
3. Ou seja, como `%rip = 0x0601`, o valor que queremos acessar está no endereço de memória `0x0601 + 0x200a0f = 0x201010`
4. Note que o *gdb* aponta o valor calculado no lado direito da instrução juntamente com o nome da variável global. Este mesmo nome apareceria quando usamos o comando `info variables`

Logo, a tradução da linha `+0` é simplesmente `var_global++`.

**Exercício**: Traduza o programa completo abaixo.

```asm
   void muda_valor(int a, int b, int *c){
   var_global++;
   eax = 3*b;
   a = a + eax;
   *c = a;
   eax = 3*a;
   var_global += eax;
   }
```
```asm
   int var_global = 10;

   void muda_valor(int a, int b, int *c) {
      var_global++;
      *c = a + 3*b;
      var_global += *c * 3;
   }

   int main() {
      int b;
      muda_valor(1, 2, &b);
   }
```
Lembre-se de que, ao rodar o programa, os endereços calculados podem mudar. Ou seja, na hora de analisar o programa rodando usando o *gdb* é sempre melhor usar o comando `b` para parar o programa onde você quiser e o comando `x` para mostrar dados na memória.

# Parte 2 - exercícios intermediários

Os exercícios desta seção exercitam mais de um conceito ao mesmo tempo. Cada um deles é disponibilizado via um arquivo `exI.o` na pasta `10-revisao-II` no repositório de atividades. A soluções devem ser colocadas no arquivo `solucao_exI.c` correspondente. Veja as instruções em cada arquivo para garantir que está implementando a função correta.

**Importante**: cada exercício estará disponível em uma página do handout de revisão juntamente com questões "padrão" para cada assunto. Essas questões são feitas para ajudar na compreensão dos programas. Faça-as com atenção e facilite sua vida.

\newpage

**Exercício1**: A função abaixo exercita os assuntos **Variáveis globais** e **Loops**. Seu código completo está disponível no arquivo *ex1-sem-teste*.

```asm
Dump of assembler code for function ex1:
   0x0616 <+0>:	    push   %rbx
   0x0617 <+1>:	    mov    $0x0,%ebx
   0x061c <+6>:	    jmp    0x62b <ex1+21>     =>1
   0x061e <+8>:	    mov    $0x0,%eax          2<= 
   0x0623 <+13>:	callq  0x5fa <faz_algo>
   0x0628 <+18>:	add    $0x1,%ebx
   0x062b <+21>:	cmpq   $0x0,0x2009dd(%rip)    1<=              # 0x201010 <var1>
   0x0633 <+29>:	jg     0x61e <ex1+8>          =>2
   0x0635 <+31>:	mov    %ebx,%eax
   0x0637 <+33>:	pop    %rbx
   0x0638 <+34>:	retq
```

1. A função acima recebe argumentos? Ela retorna algo? Declare-a abaixo. Se houverem outras funções no arquivo, declare-as também no espaço abaixo.

Não recebe, nem a faz_algo. Retorna eax.
```asm
   int ex1(){
      [...]
      faz_algo();
      [...]
   }
```

1. Identifique todos os lugares em que uma variável global é usada.

```asm
   0x062b <+21>:	cmpq   $0x0,0x2009dd(%rip)       # 0x201010 <var1>
```

1. Use setas nas instruções de *jmp*. Você consegue identificar um loop? Entre quais linhas?

```asm
   0x061c <+6>:	   jmp    0x62b <ex1+21>         =>1
   0x061e <+8>:	   mov    $0x0,%eax              2<= 
   0x0623 <+13>:	   callq  0x5fa <faz_algo>
   0x0628 <+18>:	   add    $0x1,%ebx
   0x062b <+21>:	   cmpq   $0x0,0x2009dd(%rip)    1<=              
   0x0633 <+29>:	   jg     0x61e <ex1+8>          =>2
```

1. Qual a condição testada?

Se a variável global é maior que zero (var_global > 0).

1. Faça uma tradução usando `if+goto` de `ex1` \vspace{10em}

Usando as perguntas acima preencha o arquivo de solução no repositório e execute os testes. Você pode supor que a função `faz_algo` existe.

```asm
   long var_global;
   int ex1(){
      int ebx;
      ebx = 0;
      int eax;
      goto if1;
      if2:
      eax = 0;
      faz_algo();
      ebx ++;
      if1:
      if(var_global - 0 > 0) goto if2;
      eax = ebx;
      return eax;
   }
```

```asm
   int var_global;
   int ex1(){
      int ebx;
      ebx = 0;
      int eax;
      goto if1;
      if2:
      eax = 0;
      faz_algo();
      ebx ++;
      if1:
      if(var_global - 0 > 0) goto if2;
      eax = ebx;
      return eax;
   }
```

```asm
   int var_global;
   int ex1(){
      int ebx = 0;
      goto if1;
      if2:
      faz_algo();
      ebx ++;
      if1:
      if(var_global > 0) goto if2;
      return ebx;
   }
```

```asm
   int var_global;
   int ex1(){
      int ebx = 0;
      while(var_global > 0){
         faz_algo();
         ebx ++;
      }
      return ebx;
   }
```

**Exercício2**: A função abaixo exercita os assuntos **Ponteiros** e **Condicionais**.

```asm
Dump of assembler code for function ex2:
   0x05fa <+0>:	    mov    (%rdx),%rax
   0x05fd <+3>:	    cmp    %rax,(%rdi)
   0x0600 <+6>:	    jg     0x610 <ex2+22>        =>1
   0x0602 <+8>:	    add    $0x1,%rax
   0x0606 <+12>:	mov    %rax,(%rdx)
   0x0609 <+15>:	add    (%rdx),%rsi               2<=
   0x060c <+18>:	mov    %rsi,(%rdi)
   0x060f <+21>:	retq
   0x0610 <+22>:	lea    (%rsi,%rsi,2),%rsi        1<=
   0x0614 <+26>:	jmp    0x609 <ex2+15>            =>2
```

1. Quais argumentos são recebidos pela função? Quais são seus tipos? Existe valor de retorno? Declare a função abaixo.

longs signed * rdi, rsi e * rdx. rax é retornado.
```asm
   int ex2(long *rdi, long rsi, long *rdx)
```

1. Desenhe setas indicando o fluxo do programa. Você consegue identificar a condição testada no `if`?

Se o valor apontado por rdi é maior que rax (* rdi > rax).

1. O programa acima é um pouco bagunçado. Qual bloco de código é rodado quando a condição acima é verdadeira?

```asm
   0x0600 <+6>:	    jg     0x610 <ex2+22>        =>1
   0x0609 <+15>:	add    (%rdx),%rsi               2<=
   0x060c <+18>:	mov    %rsi,(%rdi)
   0x060f <+21>:	retq
   0x0610 <+22>:	lea    (%rsi,%rsi,2),%rsi        1<=
   0x0614 <+26>:	jmp    0x609 <ex2+15>            =>2
```

1. Traduza o programa acima linha a linha. Fique atento às instruções `MOV` cujos operandos usem parênteses!

Com base nas respostas acima escreva seu programa completo no repositório de atividades.

```asm
   int ex2(long *rdi, long rsi, long *rdx){
      long rax = *rdx;
      if(*rdi - rax > 0) goto if1;
      rax ++;
      *rdx = rax;
      if2:
      rsi += *rdx;
      *rdi = rsi;
      return rax;
      if1:
      rsi = 3*rsi;
      goto if2;
   }
```

```asm
   long ex2(long *a, long b, long *c){
      long retval = *c;
      if(*a > retval) goto if1;
      retval ++;
      *c = retval;
      if2:
      *a = b + *c;
      return retval;
      if1:
      b = 3*b;
      goto if2;
   }
```

```asm
   long ex2(long *a, long b, long *c){
      long retval = *c;
      if(*a <= retval){
         retval ++;
         *c = retval;
         *a = b + *c;
         return retval;
      }
      b = 3*b;
      *a = b + *c;
      return retval;
   }
```

**Exercício3**: A função abaixo exercita os assuntos **Ponteiros** e **Variáveis globais**.

```asm
Dump of assembler code for function ex3:
   0x05fa <+0>:	    mov    0x200a14(%rip),%eax        # 0x201014 <var1>
   0x0600 <+6>:	    lea    (%rax,%rax,4),%eax
   0x0603 <+9>:	    mov    0x200a07(%rip),%edx        # 0x201010 <var2>
   0x0609 <+15>:	lea    (%rdx,%rdx,4),%ecx
   0x060c <+18>:	lea    (%rcx,%rcx,1),%edx
   0x060f <+21>:	add    %edx,%eax
   0x0611 <+23>:	mov    %eax,(%rdi)
   0x0613 <+25>:	add    0x2009fb(%rip),%eax        # 0x201014 <var1>
   0x0619 <+31>:	mov    %eax,0x2009f1(%rip)        # 0x201010 <var2>
```

1. Identifique quantas variáveis globais existem e onde elas são usadas.

Há 2 variáveis globais.
```asm
   0x05fa <+0>:	    mov    0x200a14(%rip),%eax        # 0x201014 <var1>
   0x0603 <+9>:	    mov    0x200a07(%rip),%edx        # 0x201010 <var2>
   0x0613 <+25>:	add    0x2009fb(%rip),%eax        # 0x201014 <var1>
   0x0619 <+31>:	mov    %eax,0x2009f1(%rip)        # 0x201010 <var2>
```

1. A função acima recebe argumentos? Quantos e quais seus tipos? Ela retorna algo? Declare-a abaixo 

1. Faça uma tradução linha a linha do programa acima, levando em conta os tamanhos corretos dos dados.

Com base nas respostas acima escreva seu programa completo no repositório de atividades.


```asm
   int var1, va2;
   int ex3(long *rdi){
      int eax;
      int edx;
      int ecx;
      eax = var1;
      eax = 5*eax;
      edx = var2;
      ecx = 5*edx;
      edx = 2*ecx;
      eax += edx;
      *rdi = eax;
      eax += var1;
      var2 = eax;
   }
```

```asm
   int var1, va2;
   int ex3(long *rdi){
      *rdi = 5*var1 + 10*var2;
      10*var2 = 10*var1;
   }
```


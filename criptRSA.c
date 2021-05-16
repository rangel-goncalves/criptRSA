#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>

/*
    ** A FUNÇÃO QUE IRA GERAR A CHAVE PUBLICA SERA A [PUBLIC_KEY()]. **
No RSA as chaves são geradas da seguinte maneira:
1. Escolha de forma aleatória dois números primos grandes p e q, da ordem de 10
100
no mínimo
2. Calcule n=pq (onde p e q são números primos distintos)
3. Escolha um inteiro 'e' tal que 1 < e < (p-1)*(q-1), de forma que 'e' e (p-1)*(q-1) sejam co-primos
(ou seja, o único divisor comum seja 1)
4. Calcule d de forma que de ≡ 1 (p-1)*(q-1)), ou seja, d seja o inverso multiplicativo
de e em (p-1)*(q-1)).
Por fim, temos: A chave pública: o par (n, e,), e a chave privada: a tripla (p, q, d). <¬
**(calculamos o valor de d na função[decrypt])**

    ** A FUNÇÃO QUE IRA REALIZAR A CRIPTOGRAFIA SERA A ENCRYPT(). **
Para transformar uma mensagem numa mensagem cifrada usando achave pública ('n' e 'e') gerada
na função anterior basta fazer uma potenciação modular: (mensagem real)^e ≡ (cifra)(mod n).

**A FUNÇÃO QUE IRA REALIZAR A DRESCRIPTOGRAFIA SERA A DECRYPT().**
Para recuperar a mensagem da mensagem cifrada usando a respectiva chave privada
n e d, basta fazer outra potenciação modular: (cifra)^d ≡ (mensagem real)(mod n).
*/

void public_key();
                                    /*Funcoes utilizadas na função [public_key]*/
                unsigned long long int escolherE(unsigned long long int,unsigned long long int);
    /*[escolherE] ira avaliar se o valor digitado para 'e' esta no intervalo [1,(p-1)*(q-1)] e se o MDC(e,(p-1)*(q-1)=1)*/
                                    int ehprimo(unsigned long long int);
                /*[ehprimo] avalia se os numeros escolhidos pelo usuario são numeros primos.*/
                unsigned long long int MDCeucli(unsigned long long int, unsigned long long int);
                                    /*calcula o mdc de dois numeros utilizando o algoritmo de Euclides*/
void encrypt(); // não precisa de funçoes auxiliares é a parte mais facil
void decrypt();
                                                /*Funçoes utilizadas em [decrypt]*/
                            unsigned long long int D(unsigned long long int, unsigned long long int);
                                                        /*comgruencia linear*/
                                    /*Calcula o inverso multiplicativo  de: e mod((p-1)*(q-1))*/
                                /*d sera o ultimo valor que falta para formar nossa chave privada*/
                unsigned long long int potencia(unsigned long long int, unsigned long long int, unsigned long long int);
                                /*Ira realizar a exponenciação modular (cifra)^d ≡ (mensagem real) (mod n)*/

void encrypt()
{
    char txtentrada[100000];
    unsigned long long int txtcript[100000]; //deve ser um int pois apos o processo teremos apenas numeros.
    unsigned long long int n, e;

        printf("Voce acessou a area do programa que que ira criptorgrafar sua mensagem, para isso voce deve ter acesso a uma CHAVE PUBLICA.\n");
        short aux;
        printf("Confirma que ja possui uma CHAVE PUBLICA? (1)Sim | (0)Nao\n");  scanf("%hd", &aux);
        if (!aux)
        {
            printf("gostaria de criar uma agora mesmo? (1)Sim | (0)Nao[FINALIZA O PROGRAMA]\n");  scanf("%hd", &aux);
            if(aux){public_key();}
            else
            {
                printf("PROGRAMA FINALIZADO!.\n");
                exit(0);
            }
        }
        //
        printf("*********************************\n");
        printf("Digite a mensagem que ira ser criptografada.\n");
        setbuf(stdin, NULL);
        fgets(txtentrada, 100000, stdin);
        unsigned long long int tamanhoDotxt = strlen(txtentrada);
        for(int i =0; i<tamanhoDotxt; i++) // ate chegar no final da mensagem
            {
                txtentrada[i]=toupper(txtentrada[i]); /*coloca em maiusculo, char por char, para converter para os retriçoes da atividade*/
                txtcript[i]=txtentrada[i]; // pego os valores de cada letra na tabela ascii
                if (txtcript[i]>=65 && txtcript[i]<=90){txtcript[i]-=63;} //e coloco dentro dos valores do projeto
                else if (txtcript[i] == 32){txtcript[i]-=4;} //caso de ser um espaço
            }
            printf("%s\n", txtentrada);
           for(int i = 0; i<tamanhoDotxt-1; i++)
            {
                printf("%llu\n", txtcript[i]);
            }
        printf("*********************************\n");
        printf("Agora digite 'n' e 'e'\n");
        setbuf(stdin, NULL);
        scanf("%llu %llu", &n, &e);

        ///////*CRIPTOGRAFIA*//////

        FILE *encrypted_message;
        encrypted_message = fopen("encrypted_message.txt", "w");

        unsigned long long int powAUX;

        for(int i =0; i<tamanhoDotxt-1; i++)
        {                              /*PROCESSO DE CRIPTOGRAFIA*/
                                /*(mensagem real)^e ≡ (cifra)(mod n)*/
            powAUX=pow(txtcript[i],e);
            powAUX %= n;
            txtcript[i]= powAUX;
            fprintf(encrypted_message, "%llu ", txtcript[i]);
        }
        fclose(encrypted_message);
        printf("\a\t\t\t*******************\n");
        printf("MENSAGEM CRIPTOGRAFADA COM SUCESSO!\nRETORNANDO AO MENU PRINCIPAL.\n");
        printf("\t\t\t*******************\n");
        return;

}

void public_key()
{
    unsigned long long int p,q,e;
    short aux;

    printf("Por favor, escolha dois numeros primos não iguais, para P e Q, nessa ordem.\n");
    setbuf(stdin, NULL);
    scanf("%llu %llu", &p, &q);
    setbuf(stdin, NULL);
    while (p==q)
    {
        printf("Desculpe os valores de P e Q não devem ser iguais\nCaso deseje ESCOLHER OUTROS VALORES digite 1, para FiNALIZAR O PROGRAMA digite 0\n");
        scanf("%hd",&aux);
        if (aux)
        {
            printf("Por favor, escolha dois numeros primos não iguais, para P e Q, nessa ordem.\n");
            scanf("%llu %llu", &p, &q);
        } else if (!aux)
        {
            printf("PROGRAMA FINALIZADO!.\n");
            exit(0);
        }
    }

    while (1)
    {
        if (!ehprimo(p) || !ehprimo(q))
        {
            printf("ERRO: Pelo menos uns dos numeros não eh primo\n");
            printf("Caso deseje ESCOLHER OUTROS VALORES digite 1, para FiNALIZAR O PROGRAMA digite 0\n");
            scanf("%hd",&aux);
            if (aux)
            {
                printf("Por favor, escolha dois numeros primos não iguais, para P e Q, nessa ordem.\n");
                scanf("%llu %llu", &p, &q);
            } else if (!aux)
            {
                printf("PROGRAMA FINALIZADO!.\n");
                exit(0);
            }
        } else if (ehprimo(p) && ehprimo(q)){break;} // caso os 2 numeros sejam primos finaliza o loop
    }
            // escolher e;
        e = escolherE(p,q);
        
        FILE *key_folder;
        key_folder = fopen("public_key.txt", "w");
        if(key_folder == NULL)
    {
        // ERRO ao criar pasta
        printf("Nao foi possivel criar o TXT.\n");
        printf("Gostaria de salvar manualmente?\t(1)Sim [VAI FORNECER A CHAVE PARA O USUARIO]| (0) Nao [FINALIZA O PROGRAMA]\n");
        scanf("%hd",&aux);
        if (aux){printf("n = %llu\n e = %llu", p * q, e);}
        else{printf("PROGRAMA FINALIZADO!.\n"); exit(0);}
    }
        fprintf(key_folder, "n = %llu, e = %llu", p * q, e);
        fclose(key_folder);
        printf("\a\n********************\n");
        printf("Chave criada com sucesso!\n RETORNANDO AO MENU PRINCIPAL.\n");
        printf("\n********************\n");
        return;
    
}

void decrypt()
{
    unsigned long long int p,q,e,d,n;
    unsigned long long int tamanhostr;

    printf("Insira os valores de 'p', 'q' e 'e'.\n");
    setbuf(stdin, NULL);
    scanf("%llu%llu%llu", &p, &q, &e);
    setbuf(stdin, NULL);
    printf("Aguarde...\n");
    d= D(e, ((p - 1) * (q - 1)));
    n = p*q;

    unsigned long long int txtcript[100000];
    char txtsaida[100000];

    FILE *encrypted_message;
    encrypted_message = fopen("encrypted_message.txt", "r");

    for(int i =0; !feof(encrypted_message); i++)
    {
        fscanf(encrypted_message, "%llu", &txtcript[i]);
        tamanhostr = i;
    }
    fclose(encrypted_message);

    FILE *decrypt_message;
    decrypt_message = fopen("decrypt_message.txt", "w");

 for(int i = 0; i < tamanhostr; i++)
    {                         /*PROCESSO DE DESCRIPTOGRAFIA*/
                          /*(cifra)^d ≡ (mensagem real)(mod n)*/
        txtsaida[i] = potencia(txtcript[i], d, n);
                                    /*RETORNANDO OS VALORES DAS LETRAS NO PROJETO PARA A TABELA ASCII*/
        if(txtsaida[i] == 28) // caso de ser um espaço
        {txtsaida[i]+=4;} else {txtsaida[i]+=63;}

        fprintf(decrypt_message, "%c", txtsaida[i]);
    }
    printf("\a\t\t\t*******************\n");
    printf("CONTEUDO DECODIFICADO.\nARQUIVO .TXT COM A MENSGEM ORIGINAL FOI GERADO.\nRETORNANDO AO MENU PRINCIPAL.\n");
    printf("\t\t\t*******************\n");
    fclose(decrypt_message);
    return;
}

int main()
{
    short ans;
    printf("\t*TRABALHO DE MATEMATICA DISCRETA: CRIPTOGRAFIA RSA.\nCarregando...\n");
    //
    printf("*********************************\n");
    printf("Menu principal:\n");
    while(1)
    {
        printf("*********************************\n");
        printf("\t |Para GERAR UMA CHAVE PUBLICA digite (1).|\n\t |Para EMCRIPITAR uma mensagem digite (2).|\n\t |Para DESENCRIPTAR uma mensagem digite (3).|\n");
        printf("\t |SAIR(0)|\n");
        scanf("%hd", &ans);
        if (ans == 1)
        {
            printf("*********************************\n");
            printf("Opcao: GERAR UMA CHAVE PUBLICA.\n");
            public_key();
        } else if (ans == 2) 
        {
            printf("*********************************\n");
            printf("Opcao: EMCRIPITAR.\n");
            encrypt();
        } else if (ans == 3)
        {
            printf("*********************************\n");
            printf("Opcao: DESENCRIPTAR.\n");
            decrypt();
        } else if (ans == 0){printf("FINALIZANDO PROGRAMA\n"); break;}
        else {printf("OPÇÃO INVALIDA\n");}
    }
    return 0;
}

unsigned long long int escolherE(unsigned long long int p,unsigned long long int q)
{
    unsigned long long int e; printf("Escolha um valor para e\n");
    scanf("%llu", &e);
    short aux;
    while(1)
    {
    if (e<=1 || e>=(p-1)*(q-1) ||MDCeucli(e,(p-1)*(q-1))!=1)
    {
        printf("ERRO: NUMERO INVALIDO\n e pode não esta no intervalo [1,(p-1)*(q-1)]\n Ou e porde não ser coprimo de (p-1)*(q-1)\n");
        printf("Caso deseje ESCOLHER OUTRO VALOR digite 1, para FiNALIZAR O PROGRAMA digite 0\n");
        scanf("%hd",&aux);
        if (aux){printf("Escolha um valor para e\n"); scanf("%llu", &e);}
        else{printf("PROGRAMA FINALIZADO!.\n"); exit(0);}
    }
    break;
    }
    return e;
}

unsigned long long int MDCeucli(unsigned long long int x, unsigned long long int y)
{
    unsigned long long int r;
    r = x % y;
    if(r == 0) {
       return y;
    }
    else {
        return MDCeucli(y, r); 
    }
}

int ehprimo(unsigned long long int x)
{
    int resu=1;
    for (unsigned long long int i =2; i< x; i++)
{
    if (x%i==0){resu=0;}
}
    return resu;
}

unsigned long long int D(unsigned long long int x, unsigned long long int modulo)
{                       //usando comgruencia linear
        unsigned long long int ans, d=1;

   while(1)
    {
       ans = (d * x) % modulo; // Repetimos essa operação ate encontrar um valor d tal que dx ≡ 1 % ((p-1)(q-1))
        if(ans == 1) { // assim que entrar a gente finaliza o ciclo e retorna 
            return d;
        } 
    d++;
    }
}

unsigned long long int potencia(unsigned long long int a, unsigned long long int e, unsigned long long int n){
	            //Calcula a forma reduzida de a^e módulo n, usando a expansão binária do expoente
	long long int A = a, P = 1, E = e;
	while(1){
		//Chegou ao fim da expansão, retorna o P
		if(E == 0)
			return P;
		//Se o expoente é ímpar
		else if(E%2 != 0){
			//Realizamos a redução módulo n de cada uma das multpilicações 			
			P = (A * P)%n;
			E = (E-1)/2;
		}
		//Se o expoente é par
		else{
			E = E/2;
		}
		//Obtém a sequência de potências
		A = (A*A)%n;
	}
    }

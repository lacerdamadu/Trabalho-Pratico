#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "SistemaDeControle.h"



#define STRING 100
#define Data 11

const double PESOMAXIMO = 10000.0;

void Central(TSondas *ListaSondas){
    setlocale(LC_ALL, "Portuguese_Brazil");
    printf("Bem-vindo(a) ao Sistema de Controle e Cataloga��o de Rochas Minerais!\n");

    printf("Como voce deseja realizar a entrada de dados?\n");
    printf("(1)Arquivo.\n(2)Terminal.\n");
    int escolha;
    scanf("%d", &escolha);
    switch (escolha){
        case 1:
            LeituraPorArquivo(ListaSondas);
            break;
        case 2:
            LeituraPeloTerminal(ListaSondas);
            break;
        default:
            break;
    }
}


int LeituraPorArquivo(TSondas *ListaSondas){

    char data[Data];
    time_t mytime;
    mytime = time(NULL);

    printf("Digite o nome do arquivo: ");
    char nome[STRING];
    scanf("%s", nome);

    FILE* entrada;
    entrada = fopen(nome, "r");
    if(entrada==NULL){
        printf("Erro ao ler o arquivo.");
        return 0;
    }

    int numsondas;
    double lat_i, long_i;
    int  c_i, v_i, vc_i, identificador;

    fscanf(entrada,"%d", &numsondas);
    FazListaVazia(ListaSondas);
    for(int i = 0; i<numsondas;i++){
        Sonda NovaSonda;
        identificador = i+1;
        fscanf(entrada,"%lf %lf %d %d %d", &lat_i, &long_i, &c_i, &v_i, &vc_i);
        InicializaSonda(&NovaSonda, lat_i, long_i, c_i, v_i, vc_i, identificador);
        InsereSonda(ListaSondas, &NovaSonda);
    }

    int numoperacoes;
    char operacao;
    double latrocha, longrocha, pesorocha; 
    char mineral1[STRING];
    char mineral2[STRING];
    char mineral3[STRING];
    
    fscanf(entrada,"%d", &numoperacoes);
    ListaMinerais ListaMineirais;
    IniVListaM(&ListaMineirais);
    for(int i=0;i<numoperacoes;i++){

        double Distancias[numsondas];
        fscanf(entrada, "%c", operacao);
        switch(operacao){

        case 'R':
            fscanf(entrada,"%lf %lf %d %s %s %s", &latrocha, &longrocha, &pesorocha, mineral1, mineral2, mineral3);
            if(strcmp(mineral1, "/n")!=0){
                InsMineral(&ListaMineirais, mineral1);
            }
            if(strcmp(mineral2, "/n")!=0){
                InsMineral(&ListaMineirais, mineral2);
            }
            if(strcmp(mineral3, "/n")!=0){
                InsMineral(&ListaMineirais, mineral3);
            }

            RochaMineral RochaTeste;
            InicializaRocha(&RochaTeste, pesorocha, &ListaMineirais, 
             latrocha, longrocha, ctime(&mytime)); /*Inicializa uma rocha nova a 
                                                    partir da linha do arquivo*/
            
            

            Apontador AuxLis = ListaSondas->pPrimeiro;

            for(int i = 0; i > numsondas; i++){
                Distancias[i] = CalculaDist(AuxLis->Sonda, RochaTeste);
                AuxLis = AuxLis->pProx;
            } /*preenche as dist�ncias relativas das sondas at� a rocha*/

            double MenorDist = Distancias[0];
            int IndDes;

            for(int j = 0; j > numsondas; j++){
                if(MenorDist > Distancias[i]){
                    MenorDist = Distancias[i];
                    IndDes = j+1;
                }
            } /*acha o identificador da sonda de menor dist�ncia*/

            
            Apontador pAux;
            pAux = ListaSondas->pPrimeiro->pProx;

            while(pAux->Sonda.Identificador != IndDes){
                pAux = pAux->pProx;
            } /*A partir do identificador acha o ender�o de mem�ria da sonda de menor dist�ncia*/
            
            //InsereRocha(&pAux->Sonda.CompartmentoS, &RochaTeste, pAux->Sonda.CompartmentoS.PesoMax);
            /*Insere a rocha nessa sonda*/
            
            break;
        case 'I':
            ImprimeSonda(ListaSondas);
            break;
        case 'E':
            //RedistribuiRochas(ListaSondas, numsondas);
            break;
        }
    }
    fclose(entrada);
    return 0;
}

int numsondas = 0;

int LeituraPeloTerminal(TSondas *ListaSondas){

    char data[Data];
    time_t mytime;
    mytime = time(NULL);
    

    if(numsondas==0){

        printf(" Bem-vindos às central de Inicializaçao de Sondas.\n"
        "Esta operacao ir� criar uma nova sonda de acordo com os atributos digitados.\n"
        "A sonda tambem ser� ligada e disparada para o solo Marciano.\n"
        "Quantas sondas gostaria de iniciar? ");
        
        int sondas;
        scanf("%d", &sondas);
        numsondas += sondas;
        for(int s = 0; s < numsondas; s++){
            Sonda NovaSonda;
            
            double lat_i, long_i;
            int  c_i, v_i, nc_i, identificador = s+1;
            printf("Digite as informa��es da sonda %d:\n", s+1);
            getchar();

            printf("Latitude:");
            scanf("%lf", &lat_i);
            getchar();
            
        
            printf("Longitude:");
            scanf("%lf", &long_i);
            getchar();
           
            printf("Capacidade de armazenamento em kg:");
            scanf("%d", &c_i);

            printf("Velocidade:");
            scanf("%d", &v_i);

            printf("Nivel de combust�vel:");
            scanf("%d", &nc_i);

            InicializaSonda(&NovaSonda, lat_i, long_i, c_i,
             v_i, nc_i, identificador);

             printf("identificador dessa sonda: %d\n", identificador);

            InsereSonda(ListaSondas,&NovaSonda);

            LigaSonda(&NovaSonda);

        }
        printf("Inicializando informações...\n");
    }

    
    printf("Qual operaçao deseja realizar?\n");
    printf("(1)Inicializçao de rochas.\n"
    "(2)Redistribuiçao de rochas.\n"
    "(3)Impressao do estado atual das sondas.\n");
    int escolha; 
    scanf("%d", &escolha);

    if(escolha == 1){
            ListaMinerais ListaMineirais;

            IniVListaM(&ListaMineirais);

            printf("Esta operacao irá coletar a rocha de acordo com os atributos digitados,\n"
            "Alem de adicion�-la na sonda mais pr�xima.\n"
            "Quantas rochas gostaria de coletar? ");
            int numrochas;
            scanf("%d", &numrochas);
            for(int r = 0; r <numrochas; r++){
                double latrocha, longrocha, pesorocha; 
        

                printf("Digite as informa��es da rocha %d:\n", r+1);

                printf("Latitude:");
                scanf("%lf", &latrocha);
                getchar();

                printf("Longitude:");
                scanf("%lf", &longrocha);
                getchar();

                printf("Peso da rocha:");
                scanf("%lf", &pesorocha);
                getchar();
             

                char TodosMinerais[300];
                char nome[100];
                int j = 0;
                printf("Digite at� tr�s minerais separados por espa�os:\n");

                fgets(TodosMinerais, sizeof(TodosMinerais), stdin);

                for (int i = 0; i < STRING; i++) {
                    if ((TodosMinerais[i] == ' ') || (TodosMinerais[i] == '\n') || (TodosMinerais[i] == '\0')) {
                        nome[j] = '\0'; 
                        if (strlen(nome) > 0) { 
                            InsMineral(&ListaMineirais, nome);
                            j = 0; 
                        }
                        if (TodosMinerais[i] == '\n' || TodosMinerais[i] == '\0') {
                            break;
                        }   
                    } 
                    else {
                        nome[j] = TodosMinerais[i];
                        j++;
                    }
                }


                Celula RochaTeste;

                InicializaRocha(&RochaTeste.rocha, pesorocha, &ListaMineirais,
                latrocha, longrocha, ctime(&mytime));  /*Inicializa uma rocha nova a 
                                                                                partir do que o usuário digita*/  
                                                         
                double Distancias[numsondas];       /*Vetor para armazenar as distâncias das sondas em relaçao à rocha*/

                Apontador AuxLis = ListaSondas->pPrimeiro->pProx;

                Apontador Memorias[numsondas]; /*Armazenando as sondas em um vetor pra poder usar o índicie*/

                for(int i = 0; i < numsondas; i++){

                    Distancias[i] = CalculaDist(AuxLis->Sonda, RochaTeste.rocha);
                    Memorias[i] = AuxLis;
                    AuxLis = AuxLis->pProx;
                } /*Percorre a lista de sondas armazenando as dist�ncia ralativas*/

                    double MenorDist = Distancias[0];
                    int IndDes = 1;


                    for(int j = 0; j < numsondas-1; j++){
                        for(int f = 0; f <numsondas-1-j;f++){
                            if(Distancias[f+1] < Distancias[f]){
                                double aux = Distancias[f];
                                Distancias[f] = Distancias[f+1];
                                Distancias[f+1] = aux;
                                Apontador auxp = Memorias[f];
                                Memorias[f] = Memorias[f+1];
                                Memorias[f+1] = auxp;
                                //IndDes = j+1;
                            }
                        } 
                    } /*Ordena o vetor das distâncias da menor para a maior e junto as sondas*/

                        Apontador MemoriaQueQueremos;

                        MemoriaQueQueremos = Memorias[0];
    
                        for(int d = 0; d < numsondas; d++){
                            if(PesoAtual(&MemoriaQueQueremos->Sonda.CompartmentoS) + RochaTeste.rocha.Peso <= MemoriaQueQueremos->Sonda.CompartmentoS.PesoMax){
                                
                                MoveSonda(&MemoriaQueQueremos->Sonda, RochaTeste.rocha.Latitude, RochaTeste.rocha.Longitude);

                                InsereRocha(&MemoriaQueQueremos->Sonda.CompartmentoS, &RochaTeste, MemoriaQueQueremos->Sonda.CompartmentoS.PesoMax);
                                /*Insere a rocha no compartimento da sonda com menor dist�ncia*/

                                break;
                            } else {
                                MemoriaQueQueremos = Memorias[d+1];
                                }
                            }
                        
                        EsvaziaLista(&ListaMineirais);
            }
                
        printf("Deseja realizar outra opera��o?(s/n)");
        char res1;
        scanf("%c", &res1);
        if(res1 == 's'){
            LeituraPeloTerminal(ListaSondas);
        }
        else{
            return 0;
            } 

        
        

    } else if(escolha == 3) {
            printf("Esta operca��o irá imprimir as informa��es das rochas coletadas at� o momento,\n"
            "al�m da identifica��o de qual sonda ela est� armazenda.\n");

            ImprimeSonda(ListaSondas);

            printf("Deseja realizar outra opera��o?(s/n)\n");
            getchar();
            char res3;
            scanf("%c", &res3);
            if(res3 == 's'){
                LeituraPeloTerminal(ListaSondas);
            }
            else{
                return 0;
            }
        } else if(escolha == 2){
            printf("Esta fun��o ir� mover todas as sondas para o ponto (0,0) e redistrubuir as rochas entre elas.\n"
            "Com o intuito de que cada sonda fique com aproximadamente a m�dia do peso total delas.\n");

                int m = 1;

                RedistribuiRochas(ListaSondas, numsondas);


                printf("Deseja realizar outra operacao?(s/n)\n");
                getchar();
                char res3;
                scanf("%c", &res3);
                if(res3 == 's'){
                    LeituraPeloTerminal(ListaSondas);
                }
                else{
                    return 0;
                }    
        }

     
    return 0;
}

void RedistribuiRochas(TSondas *ListaSondas, int numsondas){

    double PesoTotal = 0; 

    Apontador AuxiliarSondas = ListaSondas->pPrimeiro->pProx;

    while (AuxiliarSondas != NULL){

            MoveSonda(&AuxiliarSondas->Sonda,0,0); /*Move as sondas ao ponto (0,0)*/

            PesoTotal += PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS); /*Calcula o Peso conjunto de todas as Sondas*/

            AuxiliarSondas = AuxiliarSondas->pProx; /*Percorre a Lista*/
        }



        double Med = PesoTotal/(double)numsondas;

        Compartimento ComTemporario;
        
        CriaListaRocha(&ComTemporario, PESOMAXIMO);

        AuxiliarSondas = ListaSondas->pPrimeiro->pProx;

        Celula* RochaEmQuestao;

        while(AuxiliarSondas != NULL){
        //Percorre as sondas

            RochaEmQuestao = AuxiliarSondas->Sonda.CompartmentoS.primeiro->pProx;
            //Começa da primeira rocha de cada compartimento

            while(PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS) > Med+5){
            //Itera quando o peso da sonda sendo percorrida é maior que a média + 5


                while(RochaEmQuestao->pProx != NULL){
                // Itera até chegar na penúltima rocha do compartimento
                  
                    if(VerificaSeTemQuemReceber(ListaSondas, &AuxiliarSondas->Sonda.CompartmentoS, RochaEmQuestao->rocha.Peso)){
                    //Vê se tem outra sonda pra ficar com a rocha sendo percorrida

                        Celula* RochaRemovida = RemoveRocha(&AuxiliarSondas->Sonda.CompartmentoS,
                        RochaEmQuestao);

                        InsereRocha(&ComTemporario,
                        RochaRemovida,
                        PESOMAXIMO);
                    }

                    if(PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS) <= Med + 5){
                            break;
                        }

                    RochaEmQuestao = RochaEmQuestao->pProx;
                }  

                break;
            }

            AuxiliarSondas = AuxiliarSondas->pProx;
 
        }

        Sonda *VetorSondas[numsondas];
    
        PreencheVetor(ListaSondas, VetorSondas, numsondas);
      
        AuxiliarSondas = ListaSondas->pPrimeiro->pProx;

        OrdenaPesos(VetorSondas, numsondas);

        int ver = 0;
        
        int indm = VetorSondas[0]->Identificador;

        while (AuxiliarSondas != NULL) {

            if(!VerificaListaVazia(&ComTemporario)){

                if(PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS)+ComTemporario.ultimo->rocha.Peso
                    <= AuxiliarSondas->Sonda.CompartmentoS.PesoMax &&
                     AuxiliarSondas->Sonda.Identificador == VetorSondas[ver]->Identificador){ 

                    if(VerificaRochaExistente(&AuxiliarSondas->Sonda.CompartmentoS, &ComTemporario.ultimo->rocha)){

                        TrocaRocha(&AuxiliarSondas->Sonda.CompartmentoS, &ComTemporario.ultimo->rocha);
                        Celula* RochaRemovida = RemoveRocha(&ComTemporario, ComTemporario.ultimo);

                        PreencheVetor(ListaSondas, VetorSondas, numsondas);
                        OrdenaPesos(VetorSondas, numsondas);
                        ver = 0;

                        if(VerificaListaVazia(&ComTemporario)){
                            break;
                        }
    
                    } else {

                        Celula* RochaRemovida = RemoveRocha(&ComTemporario, ComTemporario.ultimo);

                        InsereRocha(&AuxiliarSondas->Sonda.CompartmentoS,
                                    RochaRemovida, 
                                    AuxiliarSondas->Sonda.CompartmentoS.PesoMax);

                        PreencheVetor(ListaSondas, VetorSondas, numsondas);
                        OrdenaPesos(VetorSondas, numsondas);
                        ver = 0;
                        if(VerificaListaVazia(&ComTemporario)){
                            break;
                        }

                    }

                    if(!VerificaListaVazia(&ComTemporario) && AuxiliarSondas->pProx == NULL){
                    AuxiliarSondas = ListaSondas->pPrimeiro->pProx;
                    }

                } else if(PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS)+ComTemporario.ultimo->rocha.Peso
                    > AuxiliarSondas->Sonda.CompartmentoS.PesoMax &&
                     AuxiliarSondas->Sonda.Identificador == indm){
                    //Caso seja a sonda de menor peso, mas não caiba a rocha nela, colocaremos na próxima de menor peso
                    ver++;

                    if(!VerificaListaVazia(&ComTemporario) && AuxiliarSondas->pProx == NULL){
                    AuxiliarSondas = ListaSondas->pPrimeiro->pProx;
                    }

                } else {
                    AuxiliarSondas = AuxiliarSondas->pProx;
                } 
            } else {
                break;
            }

        }

        

}


int VerificaSeTemQuemReceber(TSondas *lista, Compartimento *Comp, double peso){

    int rec = 0;

    int iden;

    Apontador AuxiliarSondas = lista->pPrimeiro->pProx;

    while(AuxiliarSondas!= NULL){
        if(AuxiliarSondas->Sonda.CompartmentoS.PesoMax == Comp->PesoMax){
            iden = AuxiliarSondas->Sonda.Identificador;
        }
        AuxiliarSondas = AuxiliarSondas->pProx;
    }
    

    AuxiliarSondas = lista->pPrimeiro->pProx;

    while (AuxiliarSondas != NULL){
        if(AuxiliarSondas->Sonda.Identificador != iden){
            if(PesoAtual(&AuxiliarSondas->Sonda.CompartmentoS) + peso <= AuxiliarSondas->Sonda.CompartmentoS.PesoMax){
                rec = AuxiliarSondas->Sonda.Identificador;
                break;
            }
        }
        AuxiliarSondas = AuxiliarSondas->pProx;
    }

    return rec;
}

void OrdenaPesos(Sonda **Sondas, int numsondas){
    for(int i = 0; i< numsondas-1; i++){
        for(int j = 0; j < numsondas - 1 - i; j++){
            if(PesoAtual(&Sondas[j]->CompartmentoS)>PesoAtual(&Sondas[j+1]->CompartmentoS)){
                Sonda *auxiliar = Sondas[j];
                Sondas[j] = Sondas[j+1];
                Sondas[j+1] = auxiliar;
            }
        }
    }
}

void PreencheVetor(TSondas *Sondas, Sonda **VetorSondas, int numsondas){

    Apontador AuxiliarSondas = Sondas->pPrimeiro->pProx;

    for(int t = 0; t <numsondas; t++){
            VetorSondas[t] = &AuxiliarSondas->Sonda;
            AuxiliarSondas = AuxiliarSondas->pProx;
        }

}
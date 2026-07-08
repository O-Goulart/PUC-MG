import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.FileWriter;

public class Main {
    public static int numComparacoes = 0;

    public static void main(String[] args) {
        try {
            ColecaoRestaurantes colecao = new ColecaoRestaurantes();
            colecao.lerCsv();
            
            Restaurante[] todosRestaurantes = colecao.getRestaurantes();
            int tamanhoColecao = colecao.getTamanho();

            ArvoreAVL arvore = new ArvoreAVL();
            Scanner sc = new Scanner(System.in);
            
            if (sc.hasNextLine()) {
                String entrada = sc.nextLine().trim();
                
                while (!entrada.equals("FIM") && !entrada.equals("-1")) {
                    int idBuscado = Integer.parseInt(entrada);
                    
                    for (int i = 0; i < tamanhoColecao; i++) {
                        if (todosRestaurantes[i].getId() == idBuscado) {
                            arvore.inserir(todosRestaurantes[i]);
                            break;
                        }
                    }
                    if (sc.hasNextLine()) {
                        entrada = sc.nextLine().trim();
                    } else {
                        break;
                    }
                }
            }

            long tempoInicial = System.currentTimeMillis();
            
            if (sc.hasNextLine()) {
                String nomeBuscado = sc.nextLine().trim();
                while (!nomeBuscado.equals("FIM") && !nomeBuscado.equals("-1")) {
                    System.out.print("raiz ");
                    boolean encontrado = arvore.pesquisar(nomeBuscado);
                    if (encontrado) {
                        System.out.println("SIM");
                    } else {
                        System.out.println("NAO");
                    }
                    
                    if (sc.hasNextLine()) {
                        nomeBuscado = sc.nextLine().trim();
                    } else {
                        break;
                    }
                }
            }
            
            long tempoFinal = System.currentTimeMillis();
            sc.close();

            arvore.caminharEmOrdem();

            PrintWriter pw = new PrintWriter(new FileWriter("855217_arvore_avl.txt"));
            pw.printf("855217\t%d\t%dms\n", numComparacoes, (tempoFinal - tempoInicial));
            pw.close();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class No {
    public Restaurante elemento;
    public No esq, dir;
    public int nivel;

    public No(Restaurante elemento) {
        this(elemento, null, null, 1);
    }

    public No(Restaurante elemento, No esq, No dir, int nivel) {
        this.elemento = elemento;
        this.esq = esq;
        this.dir = dir;
        this.nivel = nivel;
    }
}

class ArvoreAVL {
    private No raiz;

    public ArvoreAVL() {
        this.raiz = null;
    }

    private int getNivel(No i) {
        return (i == null) ? 0 : i.nivel;
    }

    private void setNivel(No i) {
        i.nivel = 1 + Math.max(getNivel(i.esq), getNivel(i.dir));
    }

    public void inserir(Restaurante x) throws Exception {
        raiz = inserir(x, raiz);
    }

    private No inserir(Restaurante x, No i) throws Exception {
        if (i == null) {
            i = new No(x);
        } else {
            int comp = x.getNome().compareTo(i.elemento.getNome());
            if (comp < 0) {
                i.esq = inserir(x, i.esq);
            } else if (comp > 0) {
                i.dir = inserir(x, i.dir);
            }
        }
        return balancear(i);
    }

    private No balancear(No i) throws Exception {
        if (i != null) {
            int fator = getNivel(i.dir) - getNivel(i.esq);

            // Se estiver balanceado (fator entre -1 e 1)
            if (Math.abs(fator) <= 1) {
                setNivel(i);
            } 
            // Desbalanceado para a direita
            else if (fator == 2) {
                int fatorFilhoDir = getNivel(i.dir.dir) - getNivel(i.dir.esq);
                // Rotação Dupla: Direita-Esquerda
                if (fatorFilhoDir == -1) {
                    i.dir = rotacionarDir(i.dir);
                }
                // Rotação Simples: Esquerda
                i = rotacionarEsq(i);
            } 
            // Desbalanceado para a esquerda
            else if (fator == -2) {
                int fatorFilhoEsq = getNivel(i.esq.dir) - getNivel(i.esq.esq);
                // Rotação Dupla: Esquerda-Direita
                if (fatorFilhoEsq == 1) {
                    i.esq = rotacionarEsq(i.esq);
                }
                // Rotação Simples: Direita
                i = rotacionarDir(i);
            } else {
                throw new Exception("Erro de balanceamento.");
            }
        }
        return i;
    }

    private No rotacionarDir(No i) {
        No noEsq = i.esq;
        No noEsqDir = noEsq.dir;

        noEsq.dir = i;
        i.esq = noEsqDir;

        setNivel(i);
        setNivel(noEsq);

        return noEsq;
    }

    private No rotacionarEsq(No i) {
        No noDir = i.dir;
        No noDirEsq = noDir.esq;

        noDir.esq = i;
        i.dir = noDirEsq;

        setNivel(i);
        setNivel(noDir);

        return noDir;
    }

    public boolean pesquisar(String nome) {
        return pesquisar(nome, raiz);
    }

    private boolean pesquisar(String nome, No i) {
        if (i == null) {
            return false;
        }
        
        Main.numComparacoes++;
        int comp = nome.compareTo(i.elemento.getNome());
        
        if (comp == 0) {
            return true;
        } else if (comp < 0) {
            System.out.print("esq ");
            return pesquisar(nome, i.esq);
        } else {
            System.out.print("dir ");
            return pesquisar(nome, i.dir);
        }
    }

    public void caminharEmOrdem() {
        caminharEmOrdem(raiz);
    }

    private void caminharEmOrdem(No i) {
        if (i != null) {
            caminharEmOrdem(i.esq);
            System.out.println(i.elemento.formatar());
            caminharEmOrdem(i.dir);
        }
    }
}

class Hora {
    private int hora, minuto;
    public Hora() { this.hora = 0; this.minuto = 0; }
    public Hora(int hora, int minuto) { this.hora = hora; this.minuto = minuto; }
    public String formatar() { return String.format("%02d:%02d", hora, minuto); }
    public static Hora parseHora(String s) {
        String[] p = s.split(":");
        return new Hora(Integer.parseInt(p[0]), Integer.parseInt(p[1]));
    }
}

class Data {
    private int ano, mes, dia;
    public Data() { this.ano = 0; this.mes = 0; this.dia = 0; }
    public Data(int ano, int mes, int dia) { this.ano = ano; this.mes = mes; this.dia = dia; }
    public String formatar() { return String.format("%02d/%02d/%04d", dia, mes, ano); }
    public static Data parseData(String s) {
        String[] p = s.split("-");
        return new Data(Integer.parseInt(p[0]), Integer.parseInt(p[1]), Integer.parseInt(p[2]));
    }
}

class Restaurante {
    private int id;
    private String nome, cidade;
    private int capacidade;
    private double avaliacao;
    private String tipos_cozinha[];
    private int faixa_preco;
    private Hora horario_abertura, horario_fechamento;
    private Data data_abertura;
    private boolean aberto;

    public Restaurante() {
        this.tipos_cozinha = new String[100];
        this.horario_abertura = new Hora();
        this.horario_fechamento = new Hora();
        this.data_abertura = new Data();
    }
    
    public int getId() { return id; }
    public void setId(int id) { this.id = id; }
    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    public static Restaurante parseRestaurante(String linha){
        Restaurante dinner = new Restaurante();
        String[] dados = linha.split(",");
        
        dinner.setId(Integer.parseInt(dados[0]));
        dinner.setNome(dados[1]);
        dinner.cidade = dados[2];
        dinner.capacidade = Integer.parseInt(dados[3]);
        dinner.avaliacao = Double.parseDouble(dados[4]);
        dinner.tipos_cozinha = dados[5].split(";");
        dinner.faixa_preco = dados[6].length();
        
        String[] particao_horario = dados[7].split("-");
        dinner.horario_abertura = Hora.parseHora(particao_horario[0]);
        dinner.horario_fechamento = Hora.parseHora(particao_horario[1]);
        
        dinner.data_abertura = Data.parseData(dados[8]);
        dinner.aberto = Boolean.parseBoolean(dados[9]);
        
        return dinner;
    }

    public String formatar() {
        String tipos = "[" + String.join(",", this.tipos_cozinha) + "]";
        String preco = "";
        for (int i = 0; i < this.faixa_preco; i++) { preco += "$"; }

        return "[" + this.id + " ## " + this.nome + " ## " + this.cidade + " ## " + 
               this.capacidade + " ## " + this.avaliacao + " ## " + tipos + " ## " + 
               preco + " ## " + this.horario_abertura.formatar() + "-" + 
               this.horario_fechamento.formatar() + " ## " + 
               this.data_abertura.formatar() + " ## " + this.aberto + "]";
    }
}

class ColecaoRestaurantes{
    private int tamanho;
    private Restaurante restaurantes[];

    public ColecaoRestaurantes(){
        this.tamanho = 0;
        this.restaurantes = new Restaurante[1000];
    }
    public int getTamanho(){ return tamanho;}
    public Restaurante[] getRestaurantes(){return restaurantes;}

    public void lerCsv() {
        try {
            File arquivo = new File("/tmp/restaurantes.csv");
            Scanner sc = new Scanner(arquivo);
            if (sc.hasNextLine()) sc.nextLine(); // Pula cabeçalho
            
            while (sc.hasNextLine()) {
                String linha = sc.nextLine();
                this.restaurantes[tamanho] = Restaurante.parseRestaurante(linha);
                this.tamanho++;
            }
            sc.close();
        } catch (FileNotFoundException e) {
            System.out.println("Arquivo não encontrado!");
        }
    }
}
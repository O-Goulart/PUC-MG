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

            ArvoreDeArvore arvore = new ArvoreDeArvore();
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
                    Restaurante encontrado = arvore.pesquisar(nomeBuscado);
                    if (encontrado != null) {
                        System.out.println("SIM " + encontrado.formatar());
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

            PrintWriter pw = new PrintWriter(new FileWriter("855217_hibrida_arvore_arvore.txt"));
            pw.printf("855217\t%d\t%dms\n", numComparacoes, (tempoFinal - tempoInicial));
            pw.close();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class NoAvl {
    public Restaurante elemento;
    public NoAvl esq, dir;
    public int nivel;

    public NoAvl(Restaurante elemento) {
        this(elemento, null, null, 1);
    }

    public NoAvl(Restaurante elemento, NoAvl esq, NoAvl dir, int nivel) {
        this.elemento = elemento;
        this.esq = esq;
        this.dir = dir;
        this.nivel = nivel;
    }
}

class ArvoreAVL {
    private NoAvl raiz;

    public ArvoreAVL() {
        this.raiz = null;
    }

    private int getNivel(NoAvl i) {
        return (i == null) ? 0 : i.nivel;
    }

    private void setNivel(NoAvl i) {
        i.nivel = 1 + Math.max(getNivel(i.esq), getNivel(i.dir));
    }

    public void inserir(Restaurante x) throws Exception {
        raiz = inserir(x, raiz);
    }

    private NoAvl inserir(Restaurante x, NoAvl i) throws Exception {
        if (i == null) {
            i = new NoAvl(x);
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

    private NoAvl balancear(NoAvl i) throws Exception {
        if (i != null) {
            int fator = getNivel(i.dir) - getNivel(i.esq);

            if (Math.abs(fator) <= 1) {
                setNivel(i);
            } 
            else if (fator == 2) {
                int fatorFilhoDir = getNivel(i.dir.dir) - getNivel(i.dir.esq);
                if (fatorFilhoDir == -1) {
                    i.dir = rotacionarDir(i.dir);
                }
                i = rotacionarEsq(i);
            } 
            else if (fator == -2) {
                int fatorFilhoEsq = getNivel(i.esq.dir) - getNivel(i.esq.esq);
                if (fatorFilhoEsq == 1) {
                    i.esq = rotacionarEsq(i.esq);
                }
                i = rotacionarDir(i);
            } else {
                throw new Exception("Erro de balanceamento.");
            }
        }
        return i;
    }

    private NoAvl rotacionarDir(NoAvl i) {
        NoAvl noEsq = i.esq;
        NoAvl noEsqDir = noEsq.dir;

        noEsq.dir = i;
        i.esq = noEsqDir;

        setNivel(i);
        setNivel(noEsq);

        return noEsq;
    }

    private NoAvl rotacionarEsq(NoAvl i) {
        NoAvl noDir = i.dir;
        NoAvl noDirEsq = noDir.esq;

        noDir.esq = i;
        i.dir = noDirEsq;

        setNivel(i);
        setNivel(noDir);

        return noDir;
    }

    public Restaurante pesquisar(String nome) {
        return pesquisar(nome, raiz);
    }

    private Restaurante pesquisar(String nome, NoAvl i) {
        if (i == null) {
            return null;
        }
        
        Main.numComparacoes++;
        int comp = nome.compareTo(i.elemento.getNome());
        
        if (comp == 0) {
            return i.elemento;
        } else if (comp < 0) {
            System.out.print("esq ");
            return pesquisar(nome, i.esq);
        } else {
            System.out.print("dir ");
            return pesquisar(nome, i.dir);
        }
    }
}

class No {
    public int chave; // capacidade mod 15
    public ArvoreAVL arvoreInterna;
    public No esq, dir;

    public No(int chave) {
        this.chave = chave;
        this.arvoreInterna = new ArvoreAVL();
        this.esq = null;
        this.dir = null;
    }
}

class ArvoreDeArvore {
    private No raiz;

    public ArvoreDeArvore() {
        this.raiz = null;
    }

    public void inserir(Restaurante x) throws Exception {
        int chave = x.getCapacidade() % 15;
        raiz = inserir(chave, x, raiz);
    }

    private No inserir(int chave, Restaurante x, No i) throws Exception {
        if (i == null) {
            i = new No(chave);
            i.arvoreInterna.inserir(x);
        } else if (chave == i.chave) {
            i.arvoreInterna.inserir(x);
        } else if (chave < i.chave) {
            i.esq = inserir(chave, x, i.esq);
        } else {
            i.dir = inserir(chave, x, i.dir);
        }
        return i;
    }

    public Restaurante pesquisar(String nome) {
        return pesquisar(nome, raiz, "RAIZ");
    }

    private Restaurante pesquisar(String nome, No i, String caminhoPrimeiroNivel) {
        System.out.print(caminhoPrimeiroNivel + " ");
        
        if (i == null) {
            return null;
        }

        System.out.print("raiz ");
        Restaurante encontrado = i.arvoreInterna.pesquisar(nome);
        if (encontrado != null) {
            return encontrado;
        }

        Restaurante resEsq = pesquisar(nome, i.esq, "ESQ");
        if (resEsq != null) {
            return resEsq;
        }
        
        return pesquisar(nome, i.dir, "DIR");
    }
}

class NoBase {
    public Restaurante elemento;
    public NoBase esq, dir;

    public NoBase(Restaurante elemento) {
        this(elemento, null, null);
    }

    public NoBase(Restaurante elemento, NoBase esq, NoBase dir) {
        this.elemento = elemento;
        this.esq = esq;
        this.dir = dir;
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
    public int getCapacidade() { return capacidade; }

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
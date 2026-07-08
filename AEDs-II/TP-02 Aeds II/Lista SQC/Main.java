import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

class Lista {
    private Restaurante[] array;
    private int n;

    public Lista(int tamanho) {
        array = new Restaurante[tamanho];
        n = 0;
    }

    public void inserirInicio(Restaurante dinner) throws Exception {
        if (n >= array.length) throw new Exception("Erro ao inserir!");
        for (int i = n; i > 0; i--) {
            array[i] = array[i - 1];
        }
        array[0] = dinner;
        n++;
    }

    public void inserirFim(Restaurante dinner) throws Exception {
        if (n >= array.length) throw new Exception("Erro ao inserir!");
        array[n] = dinner;
        n++;
    }

    public void inserir(Restaurante dinner, int pos) throws Exception {
        if (n >= array.length || pos < 0 || pos > n) throw new Exception("Erro ao inserir!");
        for (int i = n; i > pos; i--) {
            array[i] = array[i - 1];
        }
        array[pos] = dinner;
        n++;
    }

    public Restaurante removerInicio() throws Exception {
        if (n == 0) throw new Exception("Erro ao remover!");
        Restaurante resp = array[0];
        n--;
        for (int i = 0; i < n; i++) {
            array[i] = array[i + 1];
        }
        return resp;
    }

    public Restaurante removerFim() throws Exception {
        if (n == 0) throw new Exception("Erro ao remover!");
        return array[--n];
    }

    public Restaurante remover(int pos) throws Exception {
        if (n == 0 || pos < 0 || pos >= n) throw new Exception("Erro ao remover!");
        Restaurante resp = array[pos];
        n--;
        for (int i = pos; i < n; i++) {
            array[i] = array[i + 1];
        }
        return resp;
    }

    public void mostrar() {
        for (int i = 0; i < n; i++) {
            // Ajuste 2: Removido o "[i]" da saída
            System.out.println(array[i].formatar());
        }
    }
}

public class Main {
    public static Restaurante buscarPorId(Restaurante[] todos, int tamanho, int id) {
        for (int i = 0; i < tamanho; i++) {
            if (todos[i].getId() == id) {
                return todos[i];
            }
        }
        return null;
    }

    public static void main(String[] args) {
        ColecaoRestaurantes colecao = new ColecaoRestaurantes();
        colecao.lerCsv();
        
        Restaurante[] todosRestaurantes = colecao.getRestaurantes();
        int tamanhoColecao = colecao.getTamanho();

        Lista lista = new Lista(1000);
        Scanner sc = new Scanner(System.in);
        
        if (sc.hasNextLine()) {
            String entrada = sc.nextLine();
            
            while (!entrada.equals("FIM") && !entrada.equals("-1")) {
                int idBuscado = Integer.parseInt(entrada);
                Restaurante dinner = buscarPorId(todosRestaurantes, tamanhoColecao, idBuscado);
                if (dinner != null) {
                    try {
                        lista.inserirFim(dinner);
                    } catch (Exception e) {}
                }
                entrada = sc.nextLine();
            }

            if (sc.hasNextLine()) {
                String linhaNum = sc.nextLine();
                if (!linhaNum.isEmpty()) {
                    int numOperacoes = Integer.parseInt(linhaNum);
                    for (int i = 0; i < numOperacoes; i++) {
                        String linhaOp = sc.nextLine();
                        String[] partes = linhaOp.split(" ");
                        String comando = partes[0];

                        try {
                            if (comando.equals("II")) {
                                int id = Integer.parseInt(partes[1]);
                                Restaurante dinner = buscarPorId(todosRestaurantes, tamanhoColecao, id);
                                if (dinner != null) lista.inserirInicio(dinner);
                            } else if (comando.equals("IF")) {
                                int id = Integer.parseInt(partes[1]);
                                Restaurante dinner = buscarPorId(todosRestaurantes, tamanhoColecao, id);
                                if (dinner != null) lista.inserirFim(dinner);
                            } else if (comando.equals("I*")) {
                                int pos = Integer.parseInt(partes[1]);
                                int id = Integer.parseInt(partes[2]);
                                Restaurante dinner = buscarPorId(todosRestaurantes, tamanhoColecao, id);
                                if (dinner != null) lista.inserir(dinner, pos);
                            } else if (comando.equals("RI")) {
                                Restaurante removido = lista.removerInicio();
                                // Ajuste 1: Removido o espaço após (R)
                                System.out.println("(R)" + removido.getNome());
                            } else if (comando.equals("RF")) {
                                Restaurante removido = lista.removerFim();
                                System.out.println("(R)" + removido.getNome());
                            } else if (comando.equals("R*")) {
                                int pos = Integer.parseInt(partes[1]);
                                Restaurante removido = lista.remover(pos);
                                System.out.println("(R)" + removido.getNome());
                            }
                        } catch (Exception e) {}
                    }
                }
            }
        }
        sc.close();
        
        lista.mostrar();
    }
}

class Hora {
    private int hora;
    private int minuto;

    public Hora() {
        this.hora = 0;
        this.minuto = 0;
    }

    public Hora(int hora, int minuto) {
        this.hora = hora;
        this.minuto = minuto;
    }

    public int getHora() { return hora; }
    public void setHora(int hora) { this.hora = hora; }
    public int getMinuto() { return minuto; }
    public void setMinuto(int minuto) { this.minuto = minuto; }
    
    public String formatar() {
        return String.format("%02d:%02d", hora, minuto);
    }

    public static Hora parseHora(String s) {
        String[] partes = s.split(":");
        return new Hora(Integer.parseInt(partes[0]), Integer.parseInt(partes[1]));
    }
}

class Data {
    private int ano;
    private int mes;
    private int dia;

    public Data() {
        this.ano = 0;
        this.mes = 0;
        this.dia = 0;
    }

    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    public int getAno() { return ano; }
    public void setAno(int ano) { this.ano = ano; }
    public int getMes() { return mes; }
    public void setMes(int mes) { this.mes = mes; }
    public int getDia() { return dia; }
    public void setDia(int dia) { this.dia = dia; }
    
    public String formatar() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }

    public static Data parseData(String s) {
        String[] partes = s.split("-");
        return new Data(Integer.parseInt(partes[0]), Integer.parseInt(partes[1]), Integer.parseInt(partes[2]));
    }
}

class Restaurante {
    private int id;
    private String nome;
    private String cidade;
    private int capacidade;
    private double avaliacao;
    private String tipos_cozinha[];
    private int faixa_preco;
    private Hora horario_abertura;
    private Hora horario_fechamento;
    private Data data_abertura;
    private boolean aberto;

    public Restaurante() {
        this.id = 0;
        this.nome = null;
        this.cidade = null;
        this.capacidade = 0;
        this.avaliacao = 0.0;
        this.tipos_cozinha = new String[100];
        this.faixa_preco = 0;
        this.horario_abertura = new Hora();
        this.horario_fechamento = new Hora();
        this.data_abertura = new Data();
        this.aberto = false;
    }

    public int getId() { return id; }
    public void setId(int id) { this.id = id; }
    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }
    public String getCidade() { return cidade; }
    public void setCidade(String cidade) { this.cidade = cidade; }
    public int getCapacidade() { return capacidade; }
    public void setCapacidade(int capacidade) { this.capacidade = capacidade; }
    public double getAvaliacao() { return avaliacao; }
    public void setAvaliacao(double avaliacao) { this.avaliacao = avaliacao; }
    public String[] getTipos_cozinha() { return tipos_cozinha; }
    public void setTipos_cozinha(String[] tipos_cozinha) { this.tipos_cozinha = tipos_cozinha; }
    public int getFaixa_preco() { return faixa_preco; }
    public void setFaixa_preco(int faixa_preco) { this.faixa_preco = faixa_preco; }
    public Hora getHorario_abertura() { return horario_abertura; }
    public void setHorario_abertura(Hora horario_abertura) { this.horario_abertura = horario_abertura; }
    public Hora getHorario_fechamento() { return horario_fechamento; }
    public void setHorario_fechamento(Hora horario_fechamento) { this.horario_fechamento = horario_fechamento; }
    public Data getData_abertura() { return data_abertura; }
    public void setData_abertura(Data data_abertura) { this.data_abertura = data_abertura; }
    public boolean getAberto() { return aberto; }
    public void setAberto(boolean aberto) { this.aberto = aberto; }

    public static Restaurante parseRestaurante(String linha){
        Restaurante dinner = new Restaurante();
        String[] dados = linha.split(",");
        
        dinner.setId(Integer.parseInt(dados[0]));
        dinner.setNome(dados[1]);
        dinner.setCidade(dados[2]);
        dinner.setCapacidade(Integer.parseInt(dados[3]));
        dinner.setAvaliacao(Double.parseDouble(dados[4]));
        dinner.setTipos_cozinha(dados[5].split(";"));
        dinner.setFaixa_preco(dados[6].length());
        
        String[] particao_horario = dados[7].split("-");
        dinner.setHorario_abertura(Hora.parseHora(particao_horario[0]));
        dinner.setHorario_fechamento(Hora.parseHora(particao_horario[1]));
        
        dinner.setData_abertura(Data.parseData(dados[8]));
        dinner.setAberto(Boolean.parseBoolean(dados[9]));
        
        return dinner;
    }

    public String formatar() {
        String tipos = "[" + String.join(",", this.tipos_cozinha) + "]";
        String preco = "";
        for (int i = 0; i < this.faixa_preco; i++) preco += "$";

        return "[" + this.id + " ## " + this.nome + " ## " + this.cidade + " ## " + 
               this.capacidade + " ## " + this.avaliacao + " ## " + tipos + " ## " + 
               preco + " ## " + this.horario_abertura.formatar() + "-" + 
               this.horario_fechamento.formatar() + " ## " + 
               this.data_abertura.formatar() + " ## " + this.aberto + "]";
    }
}

class ColecaoRestaurantes {
    private int tamanho;
    private Restaurante restaurantes[];

    public ColecaoRestaurantes() {
        this.tamanho = 0;
        this.restaurantes = new Restaurante[1000];
    }

    public int getTamanho() { return tamanho; }
    public Restaurante[] getRestaurantes() { return restaurantes; }

    public void lerCsv() {
        try {
            File arquivo = new File("/tmp/restaurantes.csv"); 
            if (!arquivo.exists()) arquivo = new File("restaurantes.csv");
            Scanner sc = new Scanner(arquivo);

            if (sc.hasNextLine()) sc.nextLine();
            
            while (sc.hasNextLine()) {
                String linha = sc.nextLine();
                if (linha.trim().isEmpty()) continue;
                Restaurante estabelecimento = Restaurante.parseRestaurante(linha);
                this.restaurantes[tamanho] = estabelecimento;
                this.tamanho++;
            }
            sc.close();
        } catch (FileNotFoundException e) {}
    }
}
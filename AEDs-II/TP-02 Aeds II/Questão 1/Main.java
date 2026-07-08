import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

public class Main {
    public static void main(String[] args) {
        ColecaoRestaurantes colecao = new ColecaoRestaurantes();
        colecao.lerCsv();
        
        Restaurante[] array = colecao.getRestaurantes();
        int tamanhoColecao = colecao.getTamanho();

        Scanner sc = new Scanner(System.in);
        
        if (sc.hasNextLine()) {
            String entrada = sc.nextLine();
            
            while (!entrada.equals("FIM") && !entrada.equals("-1")) {
                int idBuscado = Integer.parseInt(entrada);
                
                for (int i = 0; i < tamanhoColecao; i++) {
                    if (array[i].getId() == idBuscado) {
                        System.out.println(array[i].formatar());
                        break;
                    }
                }
                entrada = sc.nextLine();
            }
        }
        sc.close();
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

    public Restaurante(int id, String nome, String cidade, int capacidade, double avaliacao, String tipos_cozinha[], int faixa_preco, Hora horario_abertura, Hora horario_fechamento, Data data_abertura, boolean aberto) {
        this.id = id;
        this.nome = nome;
        this.cidade = cidade;
        this.capacidade = capacidade;
        this.avaliacao = avaliacao;
        this.tipos_cozinha = tipos_cozinha;
        this.faixa_preco = faixa_preco;
        this.horario_abertura = horario_abertura;
        this.horario_fechamento = horario_fechamento;
        this.data_abertura = data_abertura;
        this.aberto = aberto;
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
        for (int i = 0; i < this.faixa_preco; i++) {
            preco += "$";
        }

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

    public ColecaoRestaurantes(int tamanho, Restaurante restaurantes[]){
        this.tamanho = tamanho;
        this.restaurantes =  restaurantes;
    }

    public int getTamanho(){ return tamanho;}
    public void setTamanho(int tamanho){this.tamanho = tamanho;}
    
    public Restaurante[] getRestaurantes(){return restaurantes;}
    public void setRestaurantes(Restaurante restaurantes[]){this.restaurantes = restaurantes;}

    public void lerCsv() {
        try {
            //File arquivo = new File("restaurantes.csv"); 
            File arquivo = new File("/tmp/restaurantes.csv");
            Scanner sc = new Scanner(arquivo);

            if (sc.hasNextLine()) {
                sc.nextLine(); // Pula cabeçalho
            }
            
            while (sc.hasNextLine()) {
                String linha = sc.nextLine();
                Restaurante estabelecimento = Restaurante.parseRestaurante(linha);
                this.restaurantes[tamanho] = estabelecimento;
                this.tamanho++;
            }
            sc.close();

        } catch (FileNotFoundException e) {
            System.out.println("Arquivo não encontrado!");
        }
    }
}
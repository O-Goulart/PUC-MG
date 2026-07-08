import java.util.Scanner;

class Celula {
    public int elemento;
    public Celula inf, sup, esq, dir;

    public Celula() {
        this(0);
    }

    public Celula(int elemento) {
        this.elemento = elemento;
        this.inf = this.sup = this.esq = this.dir = null;
    }
}

class Matriz {
    private Celula inicio;
    private int linha, coluna;

    public Matriz() {
        this(3, 3);
    }

    public Matriz(int linha, int coluna) {
        this.linha = linha;
        this.coluna = coluna;

        if (linha > 0 && coluna > 0) {
            // Alocando a primeira coluna inteira
            inicio = new Celula();
            Celula linhaAtual = inicio;
            for (int i = 1; i < linha; i++) {
                Celula nova = new Celula();
                linhaAtual.inf = nova;
                nova.sup = linhaAtual;
                linhaAtual = linhaAtual.inf;
            }

            // Alocando as colunas restantes para cada linha (caminhando para a direita)
            linhaAtual = inicio;
            for (int i = 0; i < linha; i++) {
                Celula celulaAtual = linhaAtual;
                for (int j = 1; j < coluna; j++) {
                    Celula nova = new Celula();
                    celulaAtual.dir = nova;
                    nova.esq = celulaAtual;
                    celulaAtual = celulaAtual.dir;
                }
                linhaAtual = linhaAtual.inf;
            }

            // Conectando os ponteiros verticais (sup/inf) das colunas recém-criadas
            Celula topo = inicio;
            Celula fundo = inicio.inf;
            while (fundo != null) {
                Celula c1 = topo.dir;
                Celula c2 = fundo.dir;
                while (c1 != null && c2 != null) {
                    c1.inf = c2;
                    c2.sup = c1;
                    c1 = c1.dir;
                    c2 = c2.dir;
                }
                topo = topo.inf;
                fundo = fundo.inf;
            }
        }
    }

    public void ler(Scanner sc) {
        Celula i = inicio;
        while (i != null) {
            Celula j = i;
            while (j != null) {
                j.elemento = sc.nextInt();
                j = j.dir;
            }
            i = i.inf;
        }
    }

    public Matriz soma(Matriz m) {
        Matriz resp = null;

        if (this.linha == m.linha && this.coluna == m.coluna) {
            resp = new Matriz(this.linha, this.coluna);
            
            Celula iResp = resp.inicio;
            Celula iA = this.inicio;
            Celula iB = m.inicio;

            while (iA != null) {
                Celula jResp = iResp;
                Celula jA = iA;
                Celula jB = iB;

                while (jA != null) {
                    jResp.elemento = jA.elemento + jB.elemento;
                    
                    jResp = jResp.dir;
                    jA = jA.dir;
                    jB = jB.dir;
                }
                iResp = iResp.inf;
                iA = iA.inf;
                iB = iB.inf;
            }
        }
        return resp;
    }

    public Matriz multiplicacao(Matriz m) {
        Matriz resp = null;

        if (this.coluna == m.linha) {
            resp = new Matriz(this.linha, m.coluna);
            
            Celula iResp = resp.inicio;
            Celula iA = this.inicio;

            while (iA != null) {
                Celula jResp = iResp;
                Celula topoB = m.inicio;

                while (jResp != null) {
                    int soma = 0;
                    Celula kA = iA;
                    Celula kB = topoB;

                    // Multiplica a linha de A pela coluna de B
                    while (kA != null && kB != null) {
                        soma += kA.elemento * kB.elemento;
                        kA = kA.dir; // Avança na linha de A
                        kB = kB.inf; // Desce na coluna de B
                    }
                    
                    jResp.elemento = soma;
                    jResp = jResp.dir;
                    topoB = topoB.dir; // Avança para a próxima coluna de B
                }
                iResp = iResp.inf;
                iA = iA.inf;
            }
        }
        return resp;
    }

    public boolean isQuadrada() {
        return (this.linha == this.coluna);
    }

    public void mostrarDiagonalPrincipal() {
        if (isQuadrada()) {
            Celula i = inicio;
            while (i != null) {
                System.out.print(i.elemento + " ");
                i = i.inf;
                if (i != null) {
                    i = i.dir;
                }
            }
            System.out.println();
        }
    }

    public void mostrarDiagonalSecundaria() {
        if (isQuadrada()) {
            Celula i = inicio;
            
            while (i.dir != null) {
                i = i.dir;
            }

            while (i != null) {
                System.out.print(i.elemento + " ");
                i = i.inf;
                if (i != null) {
                    i = i.esq;
                }
            }
            System.out.println();
        }
    }

    public void mostrar() {
        Celula i = inicio;
        while (i != null) {
            Celula j = i;
            while (j != null) {
                System.out.print(j.elemento + " ");
                j = j.dir;
            }
            System.out.println();
            i = i.inf;
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        if (sc.hasNextInt()) {
            int casos = sc.nextInt();
            
            for (int c = 0; c < casos; c++) {

                int linhas = sc.nextInt();
                int colunas = sc.nextInt();
                
                Matriz m1 = new Matriz(linhas, colunas);
                m1.ler(sc);
                
                Matriz m2 = new Matriz(linhas, colunas);
                m2.ler(sc);
                
                m1.mostrarDiagonalPrincipal();
                m2.mostrarDiagonalSecundaria();
                
                Matriz soma = m1.soma(m2);
                if (soma != null) {
                    soma.mostrar();
                }
                
                Matriz multiplicacao = m1.multiplicacao(m2);
                if (multiplicacao != null) {
                    multiplicacao.mostrar();
                }
            }
        }
        sc.close();
    }
}
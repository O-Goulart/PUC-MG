import java.util.Scanner;

public class Senha{
    public static void main (String[] Args) {
        Scanner sc = new Scanner(System.in);

        while(sc.hasNextLine()){
            String senha = sc.nextLine();
            if(senha.length() < 8){System.out.println("NAO"); continue;}
            int especial = 0;
            int maiuscula = 0;
            int minuscula = 0;
            int numero = 0;
            for(int i=0; i<senha.length(); i++){
                if(senha.charAt(i) >= '0' && senha.charAt(i) <= '9'){
                    numero++;
                }
                else if(senha.charAt(i) >= 'A' && senha.charAt(i) <= 'Z'){
                     maiuscula++;
                }
                else if(senha.charAt(i) >= 'a' && senha.charAt(i) <= 'z'){
                    minuscula++;
                }
                else{
                    especial++;
                }
            }
            if(numero >= 1 && maiuscula >= 1 && minuscula >= 1 && especial >= 1){
                System.out.println("SIM");
            }
            else{
                System.out.println("NAO");
            }
        }
        sc.close();
    }
}
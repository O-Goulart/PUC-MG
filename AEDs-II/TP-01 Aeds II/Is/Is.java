import java.util.Scanner;

public class Is{
    public static void main(String[] Args){
        Scanner sc = new Scanner(System.in);

        while(sc.hasNextLine()){
            String palavra = sc.nextLine();
            boolean resp1 = true;
            boolean resp2 = true;
            boolean resp3 = true;
            boolean resp4 = true;
            resp1 = vogal(palavra);
            resp2 = consoante(palavra);
            resp3 = inteiro(palavra);
            resp4 = real(palavra);

            System.out.println((resp1 ? "SIM" : "NAO") + " " + (resp2 ? "SIM" : "NAO") + " " + (resp3 ? "SIM" : "NAO") + " " + (resp4 ? "SIM" : "NAO"));
            }
            sc.close();    
    }

     public static boolean vogal(String palavra){
            boolean resp = true;
            for(int i=0; i<palavra.length();i++){
                char c = palavra.charAt(i);
                if(c != 'A' && c != 'a' && c != 'E' && c != 'e' && c != 'I' && c != 'i' && c != 'O' && c != 'o' && c != 'U' && c != 'u'){
                    resp = false;
                    break;
                }
            }
            return resp;
        }
    
         public static boolean consoante(String palavra){
            boolean resp = true;
            for(int i=0; i<palavra.length();i++){
                char c = palavra.charAt(i);
                boolean letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
                boolean vogal = (c == 'A' || c == 'a' || c == 'E' || c == 'e' || c == 'I' || c == 'i' || c == 'O' || c == 'o' || c == 'U' || c == 'u');
                if(!letra || vogal){ 
                    resp = false;
                    break;
                }
            }
            return resp;
        }

         public static boolean inteiro(String palavra){
            boolean resp = true;
            for(int i=0; i<palavra.length();i++){
                char c = palavra.charAt(i);
                if(c < '0' || c > '9'){
                    resp = false;
                    break;
                }
            }
            return resp;
        }

         public static boolean real(String palavra){
            boolean resp = true;
            int acc = 0;
            for(int i=0; i<palavra.length();i++){
                char c = palavra.charAt(i);
                
                if(c == '.' || c == ','){
                        acc++;
                        if(acc > 1){resp = false; break;}
                }
                else if(c < '0' || c > '9'){
                    resp = false;
                    break;
                }
            }
            return resp;
        }
}
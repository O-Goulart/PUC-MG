import java.util.Random;
import java.util.Scanner;

public class Alteracao{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Random gerador = new Random();
        gerador.setSeed(4);
        
        while(sc.hasNext()){
            String palavra = sc.nextLine();
            if(palavra.equals("FIM")){break;}
            String nova = "";
            char base = (char)('a' + (Math.abs(gerador.nextInt())% 26));
            char substituta = (char)('a' + (Math.abs(gerador.nextInt())% 26));

            for(int i=0; i<palavra.length(); i++){
                char c = palavra.charAt(i);
                if(c == base){
                   c = substituta;
                   nova += c;
                }
                else{
                    nova += c;
                }
            }
            System.out.println(nova);
        }
    }
}
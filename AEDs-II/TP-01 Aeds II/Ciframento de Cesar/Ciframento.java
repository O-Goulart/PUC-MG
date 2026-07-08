import java.util.Scanner;

public class Ciframento{
    public static void main(String Args[]){
        Scanner sc = new Scanner(System.in);
        while(sc.hasNext()){
            
            String palavra = sc.nextLine();
            String nova = "";

            for(int j=0; j<palavra.length();j++){
                char c = palavra.charAt(j);
    
                c = (char)(c+3);
                
                nova += c;
            }
            System.out.println(nova);
        }
    }
}

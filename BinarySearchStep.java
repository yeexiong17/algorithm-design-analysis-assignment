import java.io.*;
import java.util.*;

public class BinarySearchStep {
    private ArrayList<String[]> data;
    
    public int binarySearchWithSteps(String target, PrintWriter writer) {
        int left = 0;
        int right = data.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            writer.println((mid+1) + ": " + data.get(mid)[0] + "/" + data.get(mid)[1]);
            
            System.out.println("Comparing target: '" + target + "' with data: '" + data.get(mid)[0] + "'");

            if (data.get(mid)[0].equals(target)) {
                writer.println("Target " + target + " found at index " + mid);
                return mid;
            } else if (data.get(mid)[0].compareTo(target) > 0) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        writer.println("-1\n");
        writer.println("Target " + target + " not found");
        return -1;
    }
    
    public void loadCSV(String filename) throws IOException {
        data = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        
        while ((line = reader.readLine()) != null) {
            String[] dataArray = line.split(",");
            for (int i = 0; i < dataArray.length; i++) {
                dataArray[i] = dataArray[i].trim();
            }
            data.add(dataArray);
        }
        reader.close();
    }
    
    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("Usage: java BinarySearchStep <dataset.csv> <target>");
            return;
        }
        
        String datasetFile = args[0];
        String target = args[1].trim(); 
        String outputFile = "binary_search_step_" + target + ".txt";
        
        BinarySearchStep searcher = new BinarySearchStep();
        searcher.loadCSV(datasetFile);
        
        if (searcher.data.isEmpty()) {
            System.out.println("Dataset is empty or invalid.");
            return;
        }
        
        System.out.println("Dataset size: " + searcher.data.size());
        System.out.println("First element: " + searcher.data.get(0)[0]);
        System.out.println("Last element: " + searcher.data.get(searcher.data.size()-1)[0]);
        System.out.println("Searching for target: " + target);
        
        PrintWriter writer = new PrintWriter(outputFile);
        int foundIndex = searcher.binarySearchWithSteps(target, writer);
        
        System.out.println("Target " + target + (foundIndex != -1 ? 
            " found at index " + foundIndex : 
            " not found") + ". Log saved to " + outputFile);
            
        writer.close();
    }
}

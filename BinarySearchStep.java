import java.io.*;
import java.util.*;

class Entry {
    int number;
    String text;
    
    Entry(int number, String text) {
        this.number = number;
        this.text = text;
    }
}

public class BinarySearchStep {
    private Entry[] data;
    private int size;
    
    // Custom merge sort implementation
    private void merge(Entry[] arr, int left, int mid, int right) {
        Entry[] temp = new Entry[right - left + 1];
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i].number <= arr[j].number)
                temp[k++] = arr[i++];
            else
                temp[k++] = arr[j++];
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (i = 0; i < k; i++)
            arr[left + i] = temp[i];
    }
    
    private void mergeSort(Entry[] arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    // Binary search with path logging
    public int binarySearch(int target, PrintWriter writer) {
        int left = 0, right = size - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            writer.println(mid + ": " + data[mid].number + "/" + data[mid].text);
            
            if (data[mid].number == target)
                return mid;
            else if (data[mid].number < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -1;
    }
    
    // Load data from CSV
    public void loadCSV(String filename) throws IOException {
        List<Entry> entries = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        
        while ((line = reader.readLine()) != null) {
            String[] parts = line.trim().split(",");
            if (parts.length == 2) {
                int number = Integer.parseInt(parts[0]);
                String text = parts[1];
                entries.add(new Entry(number, text));
            }
        }
        reader.close();
        
        size = entries.size();
        data = entries.toArray(new Entry[0]);
        mergeSort(data, 0, size - 1);
    }
    
    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("Usage: java BinarySearchStep <dataset.csv> <target>");
            return;
        }
        
        String datasetFile = args[0];
        int target = Integer.parseInt(args[1]);
        String outputFile = "binary_search_step_" + target + ".txt";
        
        BinarySearchStep searcher = new BinarySearchStep();
        searcher.loadCSV(datasetFile);
        
        PrintWriter writer = new PrintWriter(outputFile);
        int foundIndex = searcher.binarySearch(target, writer);
        
        if (foundIndex != -1) {
            writer.println("Target " + target + " found at index " + foundIndex);
            System.out.println("Target " + target + " found at index " + foundIndex + 
                             ". Log saved to " + outputFile);
        } else {
            writer.println("Target " + target + " not found");
            System.out.println("Target " + target + " not found. Log saved to " + outputFile);
        }
        writer.close();
    }
}

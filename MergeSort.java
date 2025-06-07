import java.io.*;
import java.util.*;

public class MergeSort {
    static class Data {
        int number;
        String str;
        
        Data(int number, String str) {
            this.number = number;
            this.str = str;
        }
    }
    
    public static void main(String[] args) {
        String inputFile = "dataset_1000.csv";
        String outputFile = "merge_sort_1000000.csv";
        List<Data> data = new ArrayList<>();
        
        // Read input
        long readStart = System.nanoTime();
        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                // Split into number and string parts
                int commaIndex = line.indexOf(',');
                if (commaIndex != -1) {
                    try {
                        int num = Integer.parseInt(line.substring(0, commaIndex));
                        String str = line.substring(commaIndex + 1);
                        data.add(new Data(num, str));
                    } catch (NumberFormatException e) {
                        System.err.println("Skipping invalid number in line: " + line);
                    }
                } else {
                    System.err.println("Skipping malformed line (no comma): " + line);
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading file: " + e.getMessage());
            return;
        }
        long readEnd = System.nanoTime();
        
        // Convert to array for sorting
        Data[] arr = data.toArray(new Data[0]);
        
        // Sort and time
        long sortStart = System.nanoTime();
        mergeSort(arr, 0, arr.length - 1);
        long sortEnd = System.nanoTime();
        
        // Write output
        long writeStart = System.nanoTime();
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
            for (Data item : arr) {
                bw.write(item.number + "," + item.str);
                bw.newLine();
            }
        } catch (IOException e) {
            System.err.println("Error writing file: " + e.getMessage());
        }
        long writeEnd = System.nanoTime();
        
        // Calculate times
        double readTime = (readEnd - readStart) / 1e6;
        double sortTime = (sortEnd - sortStart) / 1e6;
        double writeTime = (writeEnd - writeStart) / 1e6;
        double totalTime = readTime + sortTime + writeTime;
        
        System.out.println("Java Merge Sort Times (ms):");
        System.out.println("Input file: " + inputFile);
        System.out.println("Output file: " + outputFile);
        System.out.printf("Records processed: %,d%n", arr.length);
        System.out.printf("Read time: %.3f ms%n", readTime);
        System.out.printf("Sort time: %.3f ms%n", sortTime);
        System.out.printf("Write time: %.3f ms%n", writeTime);
        System.out.printf("Total time: %.3f ms%n", totalTime);
    }
    
    private static void mergeSort(Data[] arr, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
            merge(arr, l, m, r);
        }
    }
    
    private static void merge(Data[] arr, int l, int m, int r) {
        int n1 = m - l + 1;
        int n2 = r - m;
        Data[] L = new Data[n1];
        Data[] R = new Data[n2];
        
        System.arraycopy(arr, l, L, 0, n1);
        System.arraycopy(arr, m + 1, R, 0, n2);
        
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i].number <= R[j].number) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }
}

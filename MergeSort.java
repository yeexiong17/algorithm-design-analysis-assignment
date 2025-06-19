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
        if (args.length < 1 || args.length > 2) {
            System.err.println("Usage: java MergeSort <input_file> [output_file]");
            System.err.println("If output_file is not provided, the output will be written to");
            System.err.println("a file named <input_base>_sorted.csv in the same directory.");
            System.exit(1);
        }
        
        String inputFile = args[0];
        String outputFile;
        
        if (args.length == 2) {
            outputFile = args[1];
        } else {
           
            File inFile = new File(inputFile);
            String baseName = inFile.getName();
            String nameWithoutExtension;
            
            int dotIndex = baseName.lastIndexOf('.');
            if (dotIndex > 0) {  // remove extension if there no first character after dot
                nameWithoutExtension = baseName.substring(0, dotIndex);
            } else {
                nameWithoutExtension = baseName;
            }
            
            outputFile = new File(inFile.getParent(), nameWithoutExtension + "_sorted.csv").toString();
        }
        
        List<Data> data = new ArrayList<>();
        
        // read the input
        long readStart = System.nanoTime();
        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = br.readLine()) != null) {
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
        
        // array converter for sorting
        Data[] arr = data.toArray(new Data[0]);
        
        // Sort and time
        long sortStart = System.nanoTime();
        mergeSort(arr, 0, arr.length - 1);
        long sortEnd = System.nanoTime();
        
        // output display
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
        
        // Calculate running time
        double readTime = (readEnd - readStart) / 1e9;  // Convert to seconds
        double sortTime = (sortEnd - sortStart) / 1e9;
        double writeTime = (writeEnd - writeStart) / 1e9;
        double totalTime = readTime + sortTime + writeTime;
        
        System.out.println("Java Merge Sort Times (seconds):");
        System.out.println("Input file: " + inputFile);
        System.out.println("Output file: " + outputFile);
        System.out.printf("Records processed: %,d%n", arr.length);
        System.out.printf("Read time: %.6f s%n", readTime);
        System.out.printf("Sort time: %.6f s%n", sortTime);
        System.out.printf("Write time: %.6f s%n", writeTime);
        System.out.printf("Total time: %.6f s%n", totalTime);
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

import java.io.*;
import java.util.*;

public class QuickSort {

    // Read CSV data into a list
    public static List<String[]> readCSV(String filename) throws IOException {
        List<String[]> data = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = br.readLine()) != null) {
            data.add(line.split(","));
        }
        br.close();
        return data;
    }

    // Write sorted data to output file
    public static void writeSortedData(List<String[]> data, String filename) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
        for (String[] row : data) {
            bw.write(row[0] + "," + row[1]);
            bw.newLine();
        }
        bw.close();
    }

    // Partition function with trace
    public static int partition(List<String[]> data, int low, int high, BufferedWriter writer, int stepCounter[]) throws IOException {
        long pivot = Long.parseLong(data.get(high)[0]);  // last element as pivot
        int i = low - 1;

        StringBuilder sb = new StringBuilder("[");
        for (int k = low; k <= high; k++) {
            sb.append(data.get(k)[0]).append("/").append(data.get(k)[1]);
            if (k < high) sb.append(", ");
        }
        sb.append("] pi=").append(stepCounter[0]++);

        for (int j = low; j < high; j++) {
            if (Long.parseLong(data.get(j)[0]) <= pivot) {
                i++;
                Collections.swap(data, i, j);
            }
        }
        Collections.swap(data, i + 1, high);

        sb.append("[").append(i + 1).append("]");
        writer.write(sb.toString());
        writer.newLine();
        return i + 1;
    }

    // Quick sort with tracing
    public static void quickSortWithTrace(List<String[]> data, int low, int high, BufferedWriter writer, int stepCounter[]) throws IOException {
        if (low < high) {
            int pi = partition(data, low, high, writer, stepCounter);
            quickSortWithTrace(data, low, pi - 1, writer, stepCounter);
            quickSortWithTrace(data, pi + 1, high, writer, stepCounter);
        }
    }

    // Main function for quick_sort
    public static void main(String[] args) throws Exception {
        String input = "dataset_sample_1000.csv";
        String output = "quick_sort_1000.csv";

        List<String[]> data = readCSV(input);

        // Measure time
        long startTime = System.nanoTime();
        Collections.shuffle(data);  // Ensure unsorted before sorting
        quickSort(data, 0, data.size() - 1);
        long endTime = System.nanoTime();

        writeSortedData(data, output);

        double durationSeconds = (endTime - startTime) / 1_000_000_000.0;
        System.out.println("Quick Sort completed in " + durationSeconds + " seconds");
    }

    // Actual Quick Sort implementation
    public static void quickSort(List<String[]> data, int low, int high) {
        if (low < high) {
            int pi = partition(data, low, high);
            quickSort(data, low, pi - 1);
            quickSort(data, pi + 1, high);
        }
    }

    public static int partition(List<String[]> data, int low, int high) {
        long pivot = Long.parseLong(data.get(high)[0]);
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (Long.parseLong(data.get(j)[0]) <= pivot) {
                i++;
                Collections.swap(data, i, j);
            }
        }
        Collections.swap(data, i + 1, high);
        return i + 1;
    }
}

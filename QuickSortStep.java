import java.io.*;
import java.util.*;

public class QuickSortStep {

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

    // Utility method to format array for output
    private static String formatArray(List<String[]> data) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < data.size(); i++) {
            sb.append(data.get(i)[0]).append("/").append(data.get(i)[1]);
            if (i != data.size() - 1) sb.append(", ");
        }
        sb.append("]");
        return sb.toString();
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.out.println("Usage: java QuickSortStep <filename> <start_row> <end_row>");
            return;
        }

        String filename = args[0];
        int start = Integer.parseInt(args[1])-1;
        int end = Integer.parseInt(args[2])-1;

        List<String[]> data = readCSV(filename);

        List<String[]> subList = new ArrayList<>(data.subList(start, end + 1));

        String outputFilename = "quick_sort_step_" + (start+1) + "_" + (end+1) + ".txt";
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputFilename));

        // Write initial array state without pi= prefix
        writer.write(formatArray(subList));
        writer.newLine();

        int[] stepCounter = {0};

        QuickSort.quickSortWithTrace(subList, 0, subList.size() - 1, writer, stepCounter);

        writer.close();
        System.out.println("Step-by-step trace written to " + outputFilename);
    }
}

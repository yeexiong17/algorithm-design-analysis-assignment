import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class MergeSortStep {
    static class Data {
        int number;
        String str;
        Data(int number, String str) {
            this.number = number;
            this.str = str;
        }
    }

    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: java MergeSortStep <file_path> <start_row> <end_row>");
            System.exit(1);
        }

        String filePath = args[0];
        int startRow = Integer.parseInt(args[1]);
        int endRow = Integer.parseInt(args[2]);

        List<Data> dataList = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            int currentRow = 0;
            while ((line = br.readLine()) != null) {
                currentRow++;
                if (currentRow < startRow) continue;
                if (currentRow > endRow) break;
                
                String[] parts = line.split(",", 2);
                if (parts.length >= 2) {
                    int num = Integer.parseInt(parts[0]);
                    dataList.add(new Data(num, parts[1]));
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading file: " + e.getMessage());
            System.exit(1);
        } catch (NumberFormatException e) {
            System.err.println("Invalid number format: " + e.getMessage());
            System.exit(1);
        }

        Data[] arr = dataList.toArray(new Data[0]);
        List<String> steps = new ArrayList<>();
        steps.add(arrayToString(arr)); // Initial state

        if (arr.length > 0) {
            mergeSort(arr, 0, arr.length - 1, steps);
        }

        String stepFile = "merge_sort_step_" + startRow + "_" + endRow + ".txt";
        try (PrintWriter pw = new PrintWriter(stepFile)) {
            for (String step : steps) {
                pw.println(step);
            }
        } catch (FileNotFoundException e) {
            System.err.println("Error writing step file: " + e.getMessage());
        }

        String sortedFile = "sorted_segment_" + startRow + "_" + endRow + ".csv";
        try (PrintWriter pwSorted = new PrintWriter(sortedFile)) {
            for (Data d : arr) {
                pwSorted.println(d.number + "," + d.str);
            }
        } catch (FileNotFoundException e) {
            System.err.println("Error writing sorted segment: " + e.getMessage());
        }
    }

    private static String arrayToString(Data[] arr) {
        StringBuilder sb = new StringBuilder();
        sb.append("[");
        for (int i = 0; i < arr.length; i++) {
            if (i > 0) {
                sb.append(", ");
            }
            sb.append(arr[i].number).append("/").append(arr[i].str);
        }
        sb.append("]");
        return sb.toString();
    }

    private static void mergeSort(Data[] arr, int l, int r, List<String> steps) {
        if (l < r) {
            int mid = l + (r - l) / 2;
            mergeSort(arr, l, mid, steps);
            mergeSort(arr, mid + 1, r, steps);
            merge(arr, l, mid, r);
            steps.add(arrayToString(arr)); // Record after merge
        }
    }

    private static void merge(Data[] arr, int l, int mid, int r) {
        Data[] temp = new Data[r - l + 1];
        int i = l, j = mid + 1, k = 0;

        while (i <= mid && j <= r) {
            if (arr[i].number <= arr[j].number) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }

        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        while (j <= r) {
            temp[k++] = arr[j++];
        }

        System.arraycopy(temp, 0, arr, l, temp.length);
    }
}
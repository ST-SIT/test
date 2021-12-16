#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)


int main(int argc, char* argv[])
{
   
    char fname[100][100] = {};
    int fmax =0;
	
	//printf("引数の数: %d\n", argc);
    for (int i = 1; i < argc; i++)
    {
        printf("%d: %s\n", i, argv[i]);
        strcpy(fname[i-1],argv[i]);
 
        //文字列の長さを取得し、その長さからファイル名分、8文字を引いた場所からstrcpy
        //ファイル名だけにし、取得？

        if (fname[i-1] == NULL) {
            printf("Error\n");
            return -1;
        }
        fmax += 1;
    }	//ファイル読み取り

	
	//ファイル名の昇順にソート
	if (argc > 1) {
		for (int i = 0; i < fmax; ++i) {
			for (int j = i + 1; j < fmax; ++j) {

				char f1[100] = {};
				strncpy(f1, fname[i] + strlen(fname[i]) - 10, 6);
				char f2[100] = {};
				strncpy(f2, fname[j] + strlen(fname[j]) - 10, 6);


				if (atoi(f1) > atoi(f2)) {
					char tmp[100] = {};
					strcpy(tmp, fname[i]);
					strcpy(fname[i], fname[j]);
					strcpy(fname[j], tmp);
				}

			}
		}
	}


	//エポック数入力
	int epoch = 10000;
	//printf("Enter epochs :");
	//scanf("%d",&epoch);


	//Read result_file
		FILE* fp2;

		char fname2[100] = "../../result_" ;
		char fname_tmp[100] = {};
		sprintf(fname_tmp, "%d", epoch);
		strcat(fname2, fname_tmp);
		strcat(fname2, ".txt");

		//"./result.txt"
		fp2 = fopen(fname2, "w+");
		if (fp2 == NULL) {
			printf("%s origin_file not open!\n", fname2);
			return -1;
		}
		else {
			printf("%s origin_file opened!\n", fname2);
		}   //ファイルの読み込み確認



    //printf("%d\n",fmax);
    int fcount = 0;
	while (fcount < fmax) {

		//Read origin_file
		FILE* fp;
        
		fp = fopen(fname[fcount], "r");
		if (fp == NULL) { 
			printf("%s origin_file not open!\n", fname[fcount]);
			return -1;
		}
		else {
			printf("%s origin_file opened!\n", fname[fcount]);
		}   //ファイルの読み込み確認
		

		//char fname2[100] = {};
		////char fname3[100] = { "G:\test_drop\Debug\CR000086.txt" };
		//strncpy(fname2, fname[fcount], strlen(fname[fcount]) - 12);
		//strcat(fname2, "convert_");
		//strcat(fname2, fname[fcount] + strlen(fname[fcount]) - 12);
		////printf("%s\n", fname2);


		////Read result_file
		//FILE* fp2;

		//fp2 = fopen(fname2, "w+");
		//if (fp2 == NULL) {
		//	printf("%s origin_file not open!\n", fname2);
		//	return -1;
		//}
		//else {
		//	printf("%s origin_file opened!\n", fname2);
		//}   //ファイルの読み込み確認


        int label[100] = { 0 };
        double box_center_x[100], box_center_y[100], box_rows[100], box_cols[100], confidence[100] = { 0.0 };
        int i, j, count = 0;
        int tmp1 = 0;
        double tmp2 = 0.0;
        
        //ファイルの内容の読み取り
        while (fscanf(fp, "%d %lf %lf %lf %lf %lf", &label[count], &box_center_x[count], &box_center_y[count], &box_rows[count], &box_cols[count], &confidence[count]) != EOF)
        {
            //fscanf(fp, "%d %lf %lf %lf %lf %lf", &label, &box_center_x, &box_center_y, &box_rows, &box_cols, &confidence);
			count++;
        }

		//ラベルの昇順にソート
		for (i = 0; i < count; ++i) {
			for (j = i + 1; j < count; ++j) {

				//同じラベルの場合は高い方を採用し、低い方をラベル100でその他を0にする
				if (label[i] == label[j]) {

					if (confidence[i] > confidence[j]) {
						label[j] = 100;
						box_center_x[j] = 0.0;
						box_center_y[j] = 0.0;
						box_rows[j] = 0.0;
						box_cols[j] = 0.0;
						confidence[j] = 0.0;
					}
					else if (confidence[i] < confidence[j]) {
						label[i] = 100;
						box_center_x[i] = 0.0;
						box_center_y[i] = 0.0;
						box_rows[i] = 0.0;
						box_cols[i] = 0.0;
						confidence[i] = 0.0;
					}
				}


				if (label[i] > label[j]) {

					tmp1 = label[i];
					label[i] = label[j];
					label[j] = tmp1;

					tmp2 = box_center_x[i];
					box_center_x[i] = box_center_x[j];
					box_center_x[j] = tmp2;

					tmp2 = box_center_y[i];
					box_center_y[i] = box_center_y[j];
					box_center_y[j] = tmp2;

					tmp2 = box_rows[i];
					box_rows[i] = box_rows[j];
					box_rows[j] = tmp2;

					tmp2 = box_cols[i];
					box_cols[i] = box_cols[j];
					box_cols[j] = tmp2;

					tmp2 = confidence[i];
					confidence[i] = confidence[j];
					confidence[j] = tmp2;
				}


			}
		}

		for (i = 0; i < count; ++i) {
			if (i == 0) {
				//fprintf(fp2, "label,	box_center_x,	box_center_y,	box_rows,	box_cols,	confidence\n");
				//fprintf(fp2, "label,confidence\n");
				fprintf(fp2, "%d	%s",epoch, (fname[fcount] + strlen(fname[fcount]) - 12));
			}
			//fprintf(fp2, "%d,	%lf,	%lf,	%lf,	%lf,	%lf\n", label[i], box_center_x[i], box_center_y[i], box_rows[i], box_cols[i], confidence[i]);
			//fprintf(fp2, "%d,%lf\n", label[i], confidence[i]);

			fprintf(fp2, "	%lf", confidence[i]);
		}
		fprintf(fp2, "\n");

		fclose(fp);
		//fclose(fp2);
		fcount++;
	}

	fclose(fp2);
    //getchar();

    return(0);

}
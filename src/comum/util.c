#include "../../include/util.h"
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

static struct timeval tempo_inicio;

void registrarLog(const char* arquivo_log, const char* formato, ...) {
   FILE* fp = fopen(arquivo_log, "a");
   if (!fp) {
       fprintf(stderr, "Erro: Não foi possível abrir o arquivo de log %s\n", arquivo_log);
       return;
   }
   
   time_t agora = time(NULL);
   struct tm* tempo_local = localtime(&agora);
   char buffer_tempo[26];
   strftime(buffer_tempo, 26, "%Y-%m-%d %H:%M:%S", tempo_local);
   
   fprintf(fp, "[%s] ", buffer_tempo);
   
   va_list args;
   va_start(args, formato);
   vfprintf(fp, formato, args);
   va_end(args);
   
   fprintf(fp, "\n");
   fclose(fp);
}

void iniciarCronometro() {
   gettimeofday(&tempo_inicio, NULL);
}

double pararCronometro() {
   struct timeval tempo_fim;
   gettimeofday(&tempo_fim, NULL);
   
   double tempo_ms = (tempo_fim.tv_sec - tempo_inicio.tv_sec) * 1000.0;
   tempo_ms += (tempo_fim.tv_usec - tempo_inicio.tv_usec) / 1000.0;
   
   return tempo_ms;
}

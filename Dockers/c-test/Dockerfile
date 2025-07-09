#Use official gcc image
FROM gcc:latest
#Set work directory
WORKDIR /usr/src/app
#Copy local code to container
COPY . .
#Compile and run the C file
RUN gcc -o myprogram main.c
#Run the compiled binary
CMD ["./myprogram"]

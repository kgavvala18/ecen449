`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/12/2025 01:39:08 PM
// Design Name: 
// Module Name: jackpot
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module jackpot(SWITCHES, LEDS, CLOCK);
    input [3:0] SWITCHES;
    input CLOCK;
    output reg [3:0] LEDS;
    
    reg [31:0] clockc = 0;
    reg [3:0] lites = 4'b0001;
    reg [2:0] count = 0;
    reg clk1hz =0;
    reg game_active = 1;
    reg jackpot_mode = 0;
    
    always@ (posedge(CLOCK))
        
        begin
            clockc <= clockc + 1;
            
            if (clockc == 30000000) begin
                
                clk1hz <= ~clk1hz;
                clockc <= 0;   
                
             end
             
         end  
  
    always@ (posedge(clk1hz)) begin
        if (jackpot_mode) begin
            if (SWITCHES == 4'b0000) begin
                jackpot_mode <= 0;
                game_active <= 1;
                lites <= 4'b0001;
                count <= 0;
            end
            
        end 
        else if (game_active) begin
            if (SWITCHES == lites) begin
                LEDS <= 4'b1111;
                jackpot_mode <= 1;
                game_active <= 0;
            end
    
            else begin
            //normal led shifts
                 if (count == 3) begin
                   lites <= 4'b0001;
                   count <= 0;
                   end
                else begin
                    lites <= lites << 1;
                    count <= count + 1;
                  end
                LEDS[3:0] <= lites[3:0];
          
                end
        
            end 
       
    end
    
endmodule

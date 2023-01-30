clc; 
clear; 
close all;

s = serialport("COM11",57600);
muestras = 50000; %Cantidad de muestras que se quieren
i = 0;

datos = cell(1, 6); 
for k=1:6
    datos{k} = zeros(1, muestras);
end

% Lectura 
canal = 1;
ith_entry = 1;

% Figura
figure("Name", "Voltaje de ADC");
title("ADC", "Interpreter", "latex", "FontSize", 16);
xlabel("Time [t]",'Interpreter', 'latex', 'FontSize',12);
ylabel("Voltage [V]", "Interpreter", "latex", "FontSize", 12);
grid on; 
hold on;
time = 1:1:muestras;
ylim([0 3.5]);
xlim([0 muestras]);
counter = 1;

adc_0 = animatedline('Color', 'r');
adc_1 = animatedline('Color', 'c');
adc_2 = animatedline('Color', 'g');
adc_3 = animatedline('Color', 'm');
adc_4 = animatedline('Color', 'b');
adc_5 = animatedline('Color', 'y');

legend("Channel 0", "Channel 1", "Channel 2", "Channel 3", "Channel 8", "Channel B");

while true
    datos{1}(ith_entry) = str2double(readline(s));
    datos{2}(ith_entry) = str2double(readline(s));
    datos{3}(ith_entry) = str2double(readline(s));
    datos{4}(ith_entry) = str2double(readline(s));
    datos{5}(ith_entry) = str2double(readline(s));
    datos{6}(ith_entry) = str2double(readline(s));
    
    voltage_adc_0 = (datos{1}(ith_entry) * 3.3)/ 4095;
    voltage_adc_1 = (datos{2}(ith_entry) * 3.3)/ 4095;
    voltage_adc_2 = (datos{3}(ith_entry) * 3.3)/ 4095;
    voltage_adc_3 = (datos{4}(ith_entry) * 3.3)/ 4095;
    voltage_adc_4 = (datos{5}(ith_entry) * 3.3)/ 4095;
    voltage_adc_5 = (datos{6}(ith_entry) * 3.3)/ 4095;

    addpoints(adc_0, time(ith_entry), voltage_adc_0);
    addpoints(adc_1, time(ith_entry), voltage_adc_1);
    addpoints(adc_2, time(ith_entry), voltage_adc_2); 
    addpoints(adc_3, time(ith_entry), voltage_adc_3);
    addpoints(adc_4, time(ith_entry), voltage_adc_4);
    addpoints(adc_5, time(ith_entry), voltage_adc_5);
    drawnow

    ith_entry = ith_entry + 1;
    if ith_entry == muestras
        break
    end
end


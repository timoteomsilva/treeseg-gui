# treeseg GUI

Uma interface gráfica (GUI) para o treeseg, facilitando a seleção de programas, configuração de parâmetros e visualização de nuvens de pontos (PCD) usando PCL.

## Funcionalidades
- Menu com programas do treeseg (downsample, findstems, segmentcrown, etc.).
- Configuração de parâmetros via interface.
- Visualizador integrado para múltiplos arquivos PCD.
- Log de ações em tempo real.

## Instalação
1. Clone este repositório: `git clone https://github.com/timoteomsilva/treeseg-gui.git`
2. Instale dependências: `sudo apt install libpcl-dev pcl-tools qt5-default libvtk7-dev`
3. Compile: `mkdir build && cd build && cmake .. && make`
4. Execute: `./treeseg_gui`

## Créditos
- Baseado no [treeseg](https://github.com/apburt/treeseg) de Andrew Burt (MIT License).
- Desenvolvimento da GUI com contribuições de Grok (xAI) para ajustes e integrações.
- Licença: MIT (veja [LICENSE](LICENSE)).
## Notas
- Qt5, VTK ≥ 7, Point Cloud Library (PCL) ≥ 1.10, código-fonte do treeseg (Burt et al., 2019). Testado com sucesso no Ubuntu 20.04 LTS e 22.04 LTS. Espera-se que funcione no Ubuntu 24.04 LTS com pequenos ajustes (por exemplo, utilizando libvtk9-dev em vez de libvtk7-dev).
- Os executáveis originais do treeseg devem ser compilados separadamente e estar disponíveis no PATH do sistema ou na pasta build
## Contribuições
Pull requests são bem-vindos! Use Issues para sugestões ou bugs.

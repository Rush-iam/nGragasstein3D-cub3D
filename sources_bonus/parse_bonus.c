/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/16 00:26:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	load_wav(t_game *game)
{
	int		file_read;
	int		ret;
	char	*wav;
	char	*wav2;

	if ((file_read = open("./resources/sounds/player_noaction_bg.wav", O_RDONLY)) == -1)
		return ;
	wav = malloc(200000);
	ret = read(file_read, wav, 200000);
	wav2 = malloc(200000);
	for (int i = ret - 1; i >= 0; --i)
		wav2[ret - 1 - i] = wav[i] - 42;
	free(wav);
	cs_read_mem_wav(wav2, ret, &game->audio.sound[SND_EMITSOUND].file);
	game->audio.sound[SND_EMITSOUND].props =
						cs_make_def(&game->audio.sound[SND_EMITSOUND].file);
	free(wav2);
	close(file_read);
}

void	encode_wav()
{
	int		file_read;
	int		file_write;
	int		ret;
	char	*wav;

	if ((file_read = open("./player_beep.wav", O_RDONLY)) == -1)
		exit(0);
	wav = malloc(200000);

	if ((file_write = open("player_noaction_bg.wav", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		exit(0);
	ret = read(file_read, wav, 200000);
	for (int i = ret - 1; i >= 0; --i)
	{
		wav[i] += 42;
		write(file_write, wav + i, 1);
	}
	exit(0);
}

void	encode()
{
	int		file_read;
	int		file_write;
	int		ret;
	char	buf[1];

	if ((file_read = open(".cub", O_RDONLY)) == -1)
		exit(0);
	if ((file_write = open("enc", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		exit(0);
	while ((ret = read(file_read, buf, 1)) > 0)
	{
		if (buf[0] == '\n')
			write(file_write, "\n", 1);
		else
		{
			buf[0] = (buf[0] - 32 + 84) % 95 + 32;
			write(file_write, buf, 1);
		}
	}
	exit(0);
}

void	generate_scenefile(t_game *game, unsigned av)
{
	int		file_id;
	char	e[] = "Gt'**-t&'.+\n"
				  "8t*+!*+!*+\n"
				  ";t&&'!&&'!&&'\n"
				  ">t<ZitEhnX]ZYu\n"
				  "DtLZaatYdcZutNdjgtcZmitaZkZa/tvcZdc#XjWv\n"
				  "\n"
				  "L%t#$gZhdjgXZh$lVaah$lVaaT%#ec\\\n"
				  "L&t#$gZhdjgXZh$lVaah$lVaaT&#ec\\\n"
				  "L't#$gZhdjgXZh$lVaah$lVaaT'#ec\\\n"
				  "L(t#$gZhdjgXZh$lVaah$lVaaTlddY#ec\\\n"
				  "L)t#$gZhdjgXZh$lVaah$lVaaT)#ec\\\n"
				  "L*t#$gZhdjgXZh$lVaah$lVaaT*#ec\\\n"
				  "L+t#$gZhdjgXZh$lVaah$lVaaT+#ec\\\n"
				  "L,t#$gZhdjgXZh$lVaah$lVaaT,#ec\\\n"
				  "L-t#$gZhdjgXZh$lVaah$lVaaT-#ec\\\n"
				  "L.t#$gZhdjgXZh$lVaah$lVaaT.#ec\\\n"
				  "\n"
				  "L&%t#$gZhdjgXZh$lVaah$YddgT%#ec\\\n"
				  "L&&t#$gZhdjgXZh$lVaah$YddgT&#ec\\\n"
				  "L&'t#$gZhdjgXZh$lVaah$YddgT'#ec\\\n"
				  "L&(t#$gZhdjgXZh$lVaah$YddgT&#ec\\\n"
				  "\n"
				  "<%t#$gZhdjgXZh$lZVedch$`c^[ZT\n"
				  "<&t#$gZhdjgXZh$lZVedch$e^hidaT\n"
				  "<'t#$gZhdjgXZh$lZVedch$g^[aZT\n"
				  "\n"
				  "H%t#$gZhdjgXZh$heg^iZh$aVbe#ec\\\n"
				  "H&t#$gZhdjgXZh$heg^iZh$X]VcYZa^Zg#ec\\\n"
				  "H't#$gZhdjgXZh$heg^iZh$aVbeThiVcY#ec\\\n"
				  "H(t#$gZhdjgXZh$heg^iZh$eaVciT\\gZZc#ec\\\n"
				  "H)t#$gZhdjgXZh$heg^iZh$eaVciTYgn#ec\\\n"
				  "H*t#$gZhdjgXZh$heg^iZh$kVhZ#ec\\\n"
				  "H+t#$gZhdjgXZh$heg^iZh$[aV\\#ec\\\n"
				  "H,t#$gZhdjgXZh$heg^iZh$WVggZaTWgdlc#ec\\\n"
				  "H-t#$gZhdjgXZh$heg^iZh$WVggZaT\\gZZc#ec\\\n"
				  "H.t#$gZhdjgXZh$heg^iZh$iVWaZ#ec\\\n"
				  "H&%t#$gZhdjgXZh$heg^iZh$iVWaZTX]V^gh#ec\\\n"
				  "H&&t#$gZhdjgXZh$heg^iZh$hiVijZ#ec\\\n"
				  "H&'t#$gZhdjgXZh$heg^iZh$lZaaTZbein#ec\\\n"
				  "H&(t#$gZhdjgXZh$heg^iZh$lZaaT[jaa#ec\\\n"
				  "H&)t#$gZhdjgXZh$heg^iZh$h`ZaZidc#ec\\\n"
				  "H&*t#$gZhdjgXZh$heg^iZh$WdcZh#ec\\\n"
				  "H&+t#$gZhdjgXZh$heg^iZh$ejYYaZ#ec\\\n"
				  "H&,t#$gZhdjgXZh$heg^iZh$edi#ec\\\n"
				  "H&-t#$gZhdjgXZh$heg^iZh$\\jVgYTYZVY#ec\\\n"
				  "H&.t#$gZhdjgXZh$heg^iZh$]ZVai]Tma#ec\\\n"
				  "H'%t#$gZhdjgXZh$heg^iZh$]ZVai]Ta#ec\\\n"
				  "H'&t#$gZhdjgXZh$heg^iZh$]ZVai]Tb#ec\\\n"
				  "H''t#$gZhdjgXZh$heg^iZh$\\jcTg^[aZ#ec\\\n"
				  "H'(t#$gZhdjgXZh$heg^iZh$Vbbd#ec\\\n"
				  "H')t#$gZhdjgXZh$heg^iZh$WdcjhTma#ec\\\n"
				  "H'*t#$gZhdjgXZh$heg^iZh$WdcjhTa#ec\\\n"
				  "H'+t#$gZhdjgXZh$heg^iZh$WdcjhTb#ec\\\n"
				  "H',t#$gZhdjgXZh$heg^iZh$WdcjhTh#ec\\\n"
				  "\n"
				  "B%t#$gZhdjgXZh$bjh^X$%)t<ZitI]Zbu#lVk\n"
				  ":%t#$gZhdjgXZh$ZcZb^Zh$\\jVgY$\n"
				  "\n"
				  "6%t#$gZhdjgXZh$hdjcYh$eaVnZgTcdVXi^dc#lVk\n"
				  "6&t#$gZhdjgXZh$hdjcYh$eaVnZgTeV^c#lVk\n"
				  "6't#$gZhdjgXZh$hdjcYh$eaVnZgTYZVi]#lVk\n"
				  "6(t#$gZhdjgXZh$hdjcYh$YddgTdeZc#lVk\n"
				  "6)t#$gZhdjgXZh$hdjcYh$YddgTXadhZ#lVk\n"
				  "6*t#$gZhdjgXZh$hdjcYh$lZVedcT`c^[Z#lVk\n"
				  "6+t#$gZhdjgXZh$hdjcYh$lZVedcTe^hida#lVk\n"
				  "6,t#$gZhdjgXZh$hdjcYh$lZVedcTg^[aZ#lVk\n"
				  "6-t#$gZhdjgXZh$hdjcYh$e^X`jeTVbbd#lVk\n"
				  "6.t#$gZhdjgXZh$hdjcYh$e^X`jeTg^[aZ#lVk\n"
				  "6&%t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tb#lVk\n"
				  "6&&t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Ta#lVk\n"
				  "6&'t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tma#lVk\n"
				  "6&(t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTh#lVk\n"
				  "6&)t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTb#lVk\n"
				  "6&*t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTa#lVk\n"
				  "6&+t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTma#lVk\n"
				  "\n"
				  "tttttttttttttttttttttttttttt((()((((((()(((\n"
				  "ttttt++++++ttttttttttttttttt(!###########!(\n"
				  "ttttt+VVmm+tttttt+++++++-+++)###z#hz##z###)((((\n"
				  "ttttt+VVom++ttttt+##########(#############(mmM(\n"
				  "ttttt+VVoo,+ttttt+#S####S###q######~#####hq#lm)\n"
				  "ttttt+VVo##+ttttt+##########(#############(MMM(\n"
				  "ttttt++.++3++.++t+###+++-+++*#############*((((\n"
				  "ttttt+#mM/#/M##+t+###+tttttt(#############(\n"
				  "ttttt-#x###m#xh-++###+++tttt((()(((\"((()(((\n"
				  "ttttt+##Z######+###c###+ttttttttt(###(\n"
				  "ttttt+u###z#h##q###S###+ttttttttt(#h#(\n"
				  "ttttt+#########+#####c#+ttttttttt*#S#*\n"
				  "+++++-oxZ####x#-+++.++++ttttttttt(###(\n"
				  "+#=5++oo#######+ttttttttttttttttt(###(\n"
				  "+#S#+++.++\"++.++tttttttttttt((((((###(((\n"
				  "+###+ttt+###+ttttttttttttttt()#kZ##S##4(\n"
				  "++\"++ttt-#S#-ttttttttttttttt(V#(((###(((\n"
				  "+###+++++###+ttttttttttttttt(V=(t(###(\n"
				  "+#S#+##]+###+ttttttttttttttt(6=(t(###(\n"
				  "+#####Sl-#S#-ttttttttttttttt((((t)#S#)\n"
				  "+###+##M+#c#+tttttttttttttttttttt(###(\n"
				  "+###++++++\"+++++ttttttttttttttttt()\")(tttttttttttttttt%%%%%%%%%%\n"
				  "-#S#-+Z########+ttttttttttt++++-+0###0+-++++tttttttttt%]###T555%\n"
				  "+###++#0#####0#-ttttttttttt+0#############0+tttttttttt%#####l#5%\n"
				  "+###++##S###S##+ttttttttttt.###############.%%%%%%%%%%%########%\n"
				  "+####+#########+ttttttttttt+################%#########%########%\n"
				  "+####q#Z#######-ttttttttttt+##~####~###l~###q#########q########&\n"
				  "+####+#########+ttttttttttt+################%#########%########%\n"
				  "+###++##S###S##+ttttttttttt.###############.%%%####%%%%##w]#w#l%\n"
				  "+###++#0#####0#-ttttttttttt+0#############0+tt%%##%%tt%########%\n"
				  "+###++#######c#+ttttttttttt++,+++#####++,+++ttt%c#%ttt%######]#%\n"
				  ",#S#,+++++\"+++++tttttttttttttttt%%%\"%%%tttttttt%##%ttt%%%R%%P%%%\n"
				  "+###+ttt+###+tttttttttttttttttttt%###%ttttttttt%##%ttttt%%%%%%\n"
				  "+###+++++###+tttttttttttttttttttt%#S#%ttttttttt%##%\n"
				  "+###+h#y+###+tttttttttttttttttttt%###%ttttttttt%#c%t%%%%%%%%%\n"
				  "+#####S#-#S#-tttttttttttttttttttt%###%ttttttttt%##%%%#%#%#%]%%\n"
				  "+###+]##+###+tttttttttttttttttttt%###%ttttttttt%###%#########%\n"
				  "+#S#+++++###+tttttttttttttttttttt%#S#%ttttttttt%###q#Z#######&\n"
				  "+###+++++###+++++++++.+++tttttttt%###%ttttttttt%#c#%########s%\n"
				  "+################+#####l-tttttttt%###%ttttttttt%%%%%%#%#%#%p%%\n"
				  "+#c###S#######S##q###l##+tttttttt%#c#%tttttttttttttt%%%%%%%%%\n"
				  "+########c#######+#####l-tttttttt%#S#%\n"
				  "++++++++++,+++++++3++.+++tttttttt%###%\n"
				  "ttttttttttt-##++###=+ttttttt%%%%%%%\"%%%%%%%\n"
				  "ttttttttttt+##+##++++ttttttt%r]##%###%####%\n"
				  "ttttttttttt-#####+++tttttttt%####q#S#q#p##%\n"
				  "ttttttttttt++3++++tttttttttt%####%###%####%\n"
				  "tttttttttttt+##+tttttttttttt%####%###%####%\n"
				  "tttttttttttt++ +tttttttttttt%%%%%%###%%%%%%\n"
				  "tttttttttttt++++tttttttttttt%####%###%####%\n"
				  "tttttttttttttttttttttttttttt%:#UVq#S#q]##r%\n"
				  "tttttttttttttttttttttttttttt%####%###%####%\n"
				  "tttttttttttttttttttttttttttt%%%%%%###%%%%%%\n"
				  "tttttttttttttttttttttttttttt%#############%\n"
				  "tttttttttttttttttttttttttttt%##S###S###SlV%\n"
				  "tttttttttttttttttttttttttttt%Z#####]######%\n"
				  "tttttttttttttttttttttttttttt%&%%&%%'%%&%%&%";
	char	n[] = "Gt'**-t&'.+\n"
				  "8t'+!'+!'+\n"
				  ";t*'!+%!*'\n"
				  ">t<dt\\dt\\du\n"
				  "DtLZaatYdcZutNdjgtcZmitaZkZa/tv]dggdg#XjWv\n"
				  "\n"
				  "L%t#$gZhdjgXZh$lVaah$lVaaTWaVX`#ec\\\n"
				  "L&t#$gZhdjgXZh$lVaah$lVaaTWaVX`T_V^a#ec\\\n"
				  "L't#$gZhdjgXZh$lVaah$lVaaTWaVX`TWdcZh#ec\\\n"
				  "L(t#$gZhdjgXZh$lVaah$lVaaThidcZTcZdc#ec\\\n"
				  "L)t#$gZhdjgXZh$lVaah$lVaaThidcZTVaeVXV#ec\\\n"
				  "L*t#$gZhdjgXZh$lVaah$lVaaThidcZT`Ve^WVgV#ec\\\n"
				  "L+t#$gZhdjgXZh$lVaah$lVaaT)'TcZdc#ec\\\n"
				  "L,t#$gZhdjgXZh$lVaah$lVaaThidcZTbZYdZY#ec\\\n"
				  "L-t#$gZhdjgXZh$lVaah$lVaaThidcZThVaVbVcYgV#ec\\\n"
				  "L.t#$gZhdjgXZh$lVaah$lVaaT[^gZ#ec\\\n"
				  "\n"
				  "L&%t#$gZhdjgXZh$lVaah$YddgTcZdcT%#ec\\\n"
				  "L&&t#$gZhdjgXZh$lVaah$YddgTcZdcT&#ec\\\n"
				  "L&'t#$gZhdjgXZh$lVaah$YddgTcZdcT'#ec\\\n"
				  "L&(t#$gZhdjgXZh$lVaah$YddgTcZdcT&#ec\\\n"
				  "\n"
				  "<%t#$gZhdjgXZh$lZVedch$`c^[ZTcZdcT\n"
				  "<&t#$gZhdjgXZh$lZVedch$e^hidaTcZdcT\n"
				  "<'t#$gZhdjgXZh$lZVedch$g^[aZT\n"
				  "\n"
				  "B%t#$gZhdjgXZh$bjh^X$&%tE#D#L##lVk\n"
				  ":%t#$gZhdjgXZh$ZcZb^Zh$hh$\n"
				  "\n"
				  "6%t#$gZhdjgXZh$hdjcYh$eaVnZgTcdVXi^dc#lVk\n"
				  "6&t#$gZhdjgXZh$hdjcYh$eaVnZgTeV^c#lVk\n"
				  "6't#$gZhdjgXZh$hdjcYh$eaVnZgTYZVi]#lVk\n"
				  "6(t#$gZhdjgXZh$hdjcYh$YddgTdeZc#lVk\n"
				  "6)t#$gZhdjgXZh$hdjcYh$YddgTXadhZ#lVk\n"
				  "6*t#$gZhdjgXZh$hdjcYh$lZVedcT`c^[Z#lVk\n"
				  "6+t#$gZhdjgXZh$hdjcYh$lZVedcTe^hida#lVk\n"
				  "6,t#$gZhdjgXZh$hdjcYh$lZVedcTg^[aZ#lVk\n"
				  "6-t#$gZhdjgXZh$hdjcYh$e^X`jeTVbbd#lVk\n"
				  "6.t#$gZhdjgXZh$hdjcYh$e^X`jeTg^[aZ#lVk\n"
				  "6&%t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tb#lVk\n"
				  "6&&t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Ta#lVk\n"
				  "6&'t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tma#lVk\n"
				  "6&(t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTh#lVk\n"
				  "6&)t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTb#lVk\n"
				  "6&*t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTa#lVk\n"
				  "6&+t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTma#lVk\n"
				  "\n"
				  "H%t#$gZhdjgXZh$heg^iZh$aVbe#ec\\\n"
				  "H&t#$gZhdjgXZh$heg^iZh$X]VcYZa^Zg#ec\\\n"
				  "H't#$gZhdjgXZh$heg^iZh$aVbeThiVcY#ec\\\n"
				  "H(t#$gZhdjgXZh$heg^iZh$eaVciT\\gZZc#ec\\\n"
				  "H)t#$gZhdjgXZh$heg^iZh$eaVciTYgn#ec\\\n"
				  "H*t#$gZhdjgXZh$heg^iZh$kVhZ#ec\\\n"
				  "H+t#$gZhdjgXZh$heg^iZh$[aV\\#ec\\\n"
				  "H,t#$gZhdjgXZh$heg^iZh$WVggZaTWgdlc#ec\\\n"
				  "H-t#$gZhdjgXZh$heg^iZh$WVggZaT\\gZZc#ec\\\n"
				  "H.t#$gZhdjgXZh$heg^iZh$iVWaZ#ec\\\n"
				  "H&%t#$gZhdjgXZh$heg^iZh$iVWaZTX]V^gh#ec\\\n"
				  "H&&t#$gZhdjgXZh$heg^iZh$hiVijZ#ec\\\n"
				  "H&'t#$gZhdjgXZh$heg^iZh$lZaaTZbein#ec\\\n"
				  "H&(t#$gZhdjgXZh$heg^iZh$lZaaT[jaa#ec\\\n"
				  "H&)t#$gZhdjgXZh$heg^iZh$h`ZaZidc#ec\\\n"
				  "H&*t#$gZhdjgXZh$heg^iZh$WdcZh#ec\\\n"
				  "H&+t#$gZhdjgXZh$heg^iZh$ejYYaZ#ec\\\n"
				  "H&,t#$gZhdjgXZh$heg^iZh$edi#ec\\\n"
				  "H&-t#$gZhdjgXZh$heg^iZh$\\jVgYTYZVY#ec\\\n"
				  "\n"
				  "H&.t#$gZhdjgXZh$heg^iZh$]ZVai]Tma#ec\\\n"
				  "H'%t#$gZhdjgXZh$heg^iZh$]ZVai]Ta#ec\\\n"
				  "H'&t#$gZhdjgXZh$heg^iZh$]ZVai]Tb#ec\\\n"
				  "H''t#$gZhdjgXZh$heg^iZh$\\jcTg^[aZ#ec\\\n"
				  "H'(t#$gZhdjgXZh$heg^iZh$Vbbd#ec\\\n"
				  "H')t#$gZhdjgXZh$heg^iZh$WdcjhTma#ec\\\n"
				  "H'*t#$gZhdjgXZh$heg^iZh$WdcjhTa#ec\\\n"
				  "H'+t#$gZhdjgXZh$heg^iZh$WdcjhTb#ec\\\n"
				  "H',t#$gZhdjgXZh$heg^iZh$WdcjhTh#ec\\\n"
				  "\n"
				  "tttttttttt(((()((((((()((()(((()((((((()(((tttttt()*,-(\n"
				  "ttttt((((((Z#u####khhV~V##h=(!######h####!(tttttt(MVMm(\n"
				  "ttttt(4Vmm(######(((((((-((()###y#hy##y###)tttttt(mM(((\n"
				  "((((((V6om((#h###(V###(##(##(####%###%####(tttttt(hh(\n"
				  "(5MM5(####,(#####(#S####S###q#S##%#S#%##Sh(tttttt(##(\n"
				  "(#hh#(#####(c####(+###(##(##(###%%#h#%%###(tttttt(~~(\n"
				  "(####(()((3((+((#(o##(((-(((*#############*(((((((##((((\n"
				  "(####(#mM/#/M##(#((]](######(#z#########z#(#hh#(####h##(((((\n"
				  "+########-#m##Z##xh-((###(((((()(((\"((()(((####q##S##hhh##=&\n"
				  "(#V######q###)###x#+###h###(((((((###(tttt(####((((((###(((((\n"
				  "(########(###z#h###q###S###(6##(((#h#(((((((((((#h##u###u###(\n"
				  "(T######c(u#c######+#####c#((###(*#S#q##S###S###############(\n"
				  "(((((((((-oxZ####x#-(((+(((#####((###(((((((((((######~#####(\n"
				  "(#=5((oo#######((((%+%%%#hh#((((((###(ttttttttt(############(\n"
				  "(VS#((()((\"(()((O#%%S%#####(((((((###(((ttttttt(##~###(#(###(\n"
				  "(###(###(###(o]((##%#%#h+##(()#qh##S##R(ttttttt(#####5(S(#h#(\n"
				  "((\"((###-#S#-#S#(##%#%#####((V#(((###(((((((((((((###((#(###(\n"
				  "(###(((((###(+##(##%#%##%V#((V](t(###(#l######V(]#####(#(###((\n"
				  "(#S#(ttt(###(##((#S%#%S%%%#((V](t(###(########((((###########(\n"
				  "(#h#(ttt-#S#-###((#%#%#%###(((((t)#S#)#z######O########T###h#(\n"
				  "(###(ttt(#c#(####(#%#%#%#(((((((((###((((((((((((((((##(####V(\n"
				  "(###((((((\"(((((#(S%#%S%S(##S#])(()\")((tttt(###p(#####%%)%*%,%%%\n"
				  "-#S#-(h########(###%#%#%###((((((0###0+(((((###5(#####%]###z##5%\n"
				  "(###(((0#####0#-#####%#####(0#############0(##########q#####l##%\n"
				  "(###((##S###S##(R##########(###+#######+###(%%%%%%%%%%%%#######%\n"
				  "(####((##+-+###(((((p####((-################*###5#####%########%\n"
				  "(#S##q#l##~####-!##########q##~####~####~###q#########q###-###5&\n"
				  "(####((##+,+##((###########-###Z#######l####*###l#####%########%\n"
				  "(###((##S###S##(####(((-((((##+++#####+++##(%%%####%%%%##w]#w#l%\n"
				  "(###(((0#####0+-!##########+0#############0(%y%%##%%##%########%\n"
				  "(###((#######c#((((########(((((((###(((((((%##%c#%###%######]#%\n"
				  ",#S#,(((((\"(((((!#####(()((((##(%%,\",%%%%%%%##]%#c%#6#%%%R%%P%%%\n"
				  "+###(l#h(###(##((((+##l#######l(#%##%######y##%##%#VVV#%%%%%%%\n"
				  ")###((((-###,%%%%%%%%%%%%%%%%%%%%%#S%#%%%%%%%#%##%#V##4#m#Vh%\n"
				  "+###(ttt+####q##S#####S#####S####q##q########l%##%#%)%*%\"%%%%\n"
				  "*###(ttt-#S#,%%%%%%%%%%%%%%%%%%%%%##%###%%%#Vm%%\"%%%#%#%#%]%%\n"
				  "+###(ttt(###((+(,(((*((((((((((((%##%%%%%%%%%%%###%########V%\n"
				  ",#S#(((((###(+ k###S#####k###(ttt%#S%ttttttttt%###q#Z######5&\n"
				  "+###((,((#c#((+((-((()(((+###(ttt%##%%%%%%%%%%%#c#%#####]##s%\n"
				  "-#######+########(#####l-(#S#(ttt%##%#pm]%%%%%%#%#%##%p%T%%%%\n"
				  "(#c###S###c###S##q###l##+(###(ttt%#h%#SS#%Z#######%%\"%,%-%%%\n"
				  "(((###((###y#####(#####l-(###(ttt%#S%+###%%%%########%\n"
				  "(-,(\"((,((((((((((\"((,(((((\"((ttt%##%4###Z##k###5####%\n"
				  "(]########V-tttt(=V=(ttt(###%%%%%%%\"%%%%%%%%%%%%%%%%%%\n"
				  ",##########(tttt(((((((((#S#%(((((u#u((((((\n"
				  "(##########-ttttttt(Z#######%(]##q#S#q###p(\n"
				  "-####c#c###((((()(-(,##(*(,(%(###(###(###V(\n"
				  "(###c#c#c###*##,u#########u((4###(###(####(\n"
				  ")#####S####])+ -###########*((((\"(###(\"((((\n"
				  "(]###u#u###u+*+*####l#l#l##4(####(###(####(\n"
				  "*########l##l##l#####S#####((####(#S#(]##r(\n"
				  "((u#####l##l##l###l#l#l#l##)(####(###(####(\n"
				  "t,##c~###l#~l##l###~###u##u((####(###(####(\n"
				  "t(######l##l##l#######u()-(((#############(\n"
				  "t-uV#####l##l##l##u##h#*tttt(##S###S###S##(\n"
				  "t((u######l##l#######,-(tttt(######C######(\n"
				  "tt(((()+*+,+-+)+*+,+-+tttttt()((*((+((,((-(";
	char	h[] = "Gt'**-t&'.+\n"
				  "8t*+!*+!*+\n"
				  ";t+%!*+!++\n"
				  "9t&\n"
				  ">t6t]dgg^WaZtX]^aat\\dZhtYdlctndjgthe^cZ###\n"
				  "DtLZaatYdcZutNdjgtcZmitaZkZa/tvZm^i#XjWv\n"
				  "\n"
				  "L%t#$gZhdjgXZh$lVaah$lVaaTWaVX`#ec\\\n"
				  "L&t#$gZhdjgXZh$lVaah$lVaaTWaVX`T_V^a#ec\\\n"
				  "L't#$gZhdjgXZh$lVaah$lVaaTWaVX`TWdcZh#ec\\\n"
				  "L(t#$gZhdjgXZh$lVaah$lVaaTlddY#ec\\\n"
				  "L)t#$gZhdjgXZh$lVaah$lVaaTVaeVXV#ec\\\n"
				  "L*t#$gZhdjgXZh$lVaah$lVaaT`Ve^WVgV#ec\\\n"
				  "L+t#$gZhdjgXZh$lVaah$lVaaT)'#ec\\\n"
				  "L,t#$gZhdjgXZh$lVaah$lVaaTbZYdZY#ec\\\n"
				  "L-t#$gZhdjgXZh$lVaah$lVaaThVaVbVcYgV#ec\\\n"
				  "L.t#$gZhdjgXZh$lVaah$lVaaT[^gZ#ec\\\n"
				  "\n"
				  "L&%t#$gZhdjgXZh$lVaah$YddgTgZYT%#ec\\\n"
				  "L&&t#$gZhdjgXZh$lVaah$YddgTgZYT&#ec\\\n"
				  "L&'t#$gZhdjgXZh$lVaah$YddgT'#ec\\\n"
				  "L&(t#$gZhdjgXZh$lVaah$YddgT&#ec\\\n"
				  "\n"
				  "<%t#$gZhdjgXZh$lZVedch$`c^[ZT\n"
				  "<&t#$gZhdjgXZh$lZVedch$e^hidaT\n"
				  "<'t#$gZhdjgXZh$lZVedch$g^[aZT\n"
				  "\n"
				  "B%t#$gZhdjgXZh$bjh^X$:HTHigZhhtOdcZt\"t6aVct8Vgahdc\"<gZZct|adl}#lVk\n"
				  ":%t#$gZhdjgXZh$ZcZb^Zh$bjiVci$\n"
				  "\n"
				  "6%t#$gZhdjgXZh$hdjcYh$eaVnZgTcdVXi^dc#lVk\n"
				  "6&t#$gZhdjgXZh$hdjcYh$eaVnZgTeV^c#lVk\n"
				  "6't#$gZhdjgXZh$hdjcYh$eaVnZgTYZVi]#lVk\n"
				  "6(t#$gZhdjgXZh$hdjcYh$YddgTdeZc#lVk\n"
				  "6)t#$gZhdjgXZh$hdjcYh$YddgTXadhZ#lVk\n"
				  "6*t#$gZhdjgXZh$hdjcYh$lZVedcT`c^[Z#lVk\n"
				  "6+t#$gZhdjgXZh$hdjcYh$lZVedcTe^hida#lVk\n"
				  "6,t#$gZhdjgXZh$hdjcYh$lZVedcTg^[aZ#lVk\n"
				  "6-t#$gZhdjgXZh$hdjcYh$e^X`jeTVbbd#lVk\n"
				  "6.t#$gZhdjgXZh$hdjcYh$e^X`jeTg^[aZ#lVk\n"
				  "6&%t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tb#lVk\n"
				  "6&&t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Ta#lVk\n"
				  "6&'t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tma#lVk\n"
				  "6&(t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTh#lVk\n"
				  "6&)t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTb#lVk\n"
				  "6&*t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTa#lVk\n"
				  "6&+t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTma#lVk\n"
				  "\n"
				  "H%t#$gZhdjgXZh$heg^iZh$aVbe#ec\\\n"
				  "H&t#$gZhdjgXZh$heg^iZh$X]VcYZa^Zg#ec\\\n"
				  "H't#$gZhdjgXZh$heg^iZh$h`jaah#ec\\\n"
				  "H(t#$gZhdjgXZh$heg^iZh$eaVciT\\gZZc#ec\\\n"
				  "H)t#$gZhdjgXZh$heg^iZh$eaVciTYgn#ec\\\n"
				  "H*t#$gZhdjgXZh$heg^iZh$kVhZ#ec\\\n"
				  "H+t#$gZhdjgXZh$heg^iZh$[aV\\#ec\\\n"
				  "H,t#$gZhdjgXZh$heg^iZh$WVggZaTWgdlc#ec\\\n"
				  "H-t#$gZhdjgXZh$heg^iZh$WVggZaT\\gZZc#ec\\\n"
				  "H.t#$gZhdjgXZh$heg^iZh$_V^aTh`ZaZidch#ec\\\n"
				  "H&%t#$gZhdjgXZh$heg^iZh$iVWaZTX]V^gh#ec\\\n"
				  "H&&t#$gZhdjgXZh$heg^iZh$hiVijZTYZbdc#ec\\\n"
				  "H&'t#$gZhdjgXZh$heg^iZh$lZaaTZbein#ec\\\n"
				  "H&(t#$gZhdjgXZh$heg^iZh$lZaaTWaddY#ec\\\n"
				  "H&)t#$gZhdjgXZh$heg^iZh$h`ZaZidc#ec\\\n"
				  "H&*t#$gZhdjgXZh$heg^iZh$WdcZh#ec\\\n"
				  "H&+t#$gZhdjgXZh$heg^iZh$ejYYaZ#ec\\\n"
				  "H&,t#$gZhdjgXZh$heg^iZh$edi#ec\\\n"
				  "H&-t#$gZhdjgXZh$heg^iZh$\\jVgYTYZVY#ec\\\n"
				  "\n"
				  "H&.t#$gZhdjgXZh$heg^iZh$]ZVai]Tma#ec\\\n"
				  "H'%t#$gZhdjgXZh$heg^iZh$]ZVai]Ta#ec\\\n"
				  "H'&t#$gZhdjgXZh$heg^iZh$]ZVai]Tb#ec\\\n"
				  "H''t#$gZhdjgXZh$heg^iZh$\\jcTg^[aZ#ec\\\n"
				  "H'(t#$gZhdjgXZh$heg^iZh$Vbbd#ec\\\n"
				  "H')t#$gZhdjgXZh$heg^iZh$WdcjhTma#ec\\\n"
				  "H'*t#$gZhdjgXZh$heg^iZh$WdcjhTa#ec\\\n"
				  "H'+t#$gZhdjgXZh$heg^iZh$WdcjhTb#ec\\\n"
				  "H',t#$gZhdjgXZh$heg^iZh$WdcjhTh#ec\\\n"
				  "\n"
				  "wtttttStaVbe#ec\\\n"
				  "wttttt~tX]VcYZa^Zg#ec\\\n"
				  "wtttttxth`jaah#ec\\\n"
				  "wttttt/teaVciT\\gZZc#ec\\\n"
				  "wttttt0teaVciTYgn#ec\\\n"
				  "wttttt!tkVhZ#ec\\\n"
				  "wtttttut[aV\\#ec\\\n"
				  "wttttt5tWVggZaTWgdlc#ec\\\n"
				  "wtttttytWVggZaT\\gZZc#ec\\\n"
				  "wtttttwt_V^aTh`ZaZidch#ec\\\n"
				  "wtttttztiVWaZTX]V^gh#ec\\\n"
				  "wttttt4thiVijZTYZbdc#ec\\\n"
				  "wtttttPtlZaaTZbein#ec\\\n"
				  "wtttttRtlZaaTWaddY#ec\\\n"
				  "wtttttpth`ZaZidc#ec\\\n"
				  "wtttttrtWdcZh#ec\\\n"
				  "wtttttTtejYYaZ#ec\\\n"
				  "wtttttstedi#ec\\\n"
				  "wtttttUt\\jVgYTYZVY#ec\\\n"
				  "\n"
				  "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
				  "%%%%%%%%%###q#########4%%%%%%%%%%%%%\n"
				  "%#Sq#S]=%#%%%%%#S#]#S#4%4O]oS#%###r%\n"
				  "%##%#SV6%####%%#######4%4O]oS#qS##r%\n"
				  "%#h%%%%%%%%##%%%%%%%%%%%%%%%%%%%r##%\n"
				  "%h#%r##r%:k####0###0%%%0###0##r%r##%\n"
				  "%Sh%##SV%%%#######S#####S####Sr%##%%\n"
				  "%##%5#%%%%%%%%%%%%%%%%%%%%%%\"%%##%%%\n"
				  "%#%%####r%ZZZZZ%%rpr#pr#=pp%##%#%%\n"
				  "%#r%#####q#ZZZZ%%##########%##%#%%\n"
				  "%#S%#S###%%%%%%%%S#%%%%%%%\"%#%%S%%\n"
				  "%##%##%##%%((((%%%\"%%%%%%%#%##%#%%\n"
				  "%%#%##%5#%%Vh##%%#####rr%##%#r%#%%\n"
				  "%r#%S#%##%%SSSS%%#S#c#S#%#%%#%%##%\n"
				  "%S#%c#%#5%%##h#%%#S##cS#%##%#l%%#%\n"
				  "%##%##%##%%r##p%%p######%r#%##%%#%\n"
				  "%p#%#S%5#%%%\"%%%%&&'&&%\"%%#%#%%%S%\n"
				  "%%\"%#c%####ZS##S##Sr%%##%##%#p%%#%\n"
				  "%p#%##%r###Z###U####%%r#%#r%r#%%#%\n"
				  "%##qS#%%\"%%%%%%%%%%#%%U#%#%%%\"%##%\n"
				  "%#r%##%VS%%()(,(-(*\"(%%#%##%p#%#%%\n"
				  "%\"%%rr%##%(/S#S]S#S#+%##%U#%##%#%%\n"
				  "%#%%rr%0#%(/Sz~]~zS#+%S#%U#%##qS%%\n"
				  "%#V%&'%%\"%(##~z~z~##.%##%%#%%#%r%%\n"
				  "%\"%%rSq#S%(/Sz~]~zSl+%c#%U#%##%'%%t%%%%%%%%%%%\n"
				  "%##%&'%%%%(#S#S]S#Sl+%SU%##%P#%]%%%%S#####S##%\n"
				  "%S#%SrsrS%(\",()(*(-((%#%%#r%U#%##q###%%%%%%%S%\n"
				  "%#c%####l%%h%%%%%%%%%%#r%#%%%#%%%%%%%%r##O4%#%\n"
				  "%#c%####h%%S###S##ZS###r%h#%0#q#S#lS#q#~o=4%S%\n"
				  "%#S%S###S%%Z#SZ##S#Z###r%#h%##%%%%%%%%r##O4%#%\n"
				  "%#y%##%%\"%%%%%%%%%%%%%%%%##%%#%%#q###%%%%%%%S%\n"
				  "%##%##%%#%%%%%%%%%%%%%%%%#x%p#%%#%%%S#####S##%\n"
				  "%S#%S#%%########%#r%p#####R%r#%%#%t%%%%%%%%%%\n"
				  "%y#%##%%rSl#Sl#Sq#####%###x%%\"%##%\n"
				  "%c#%ph%%%%%%%%%%%%%%%%%%%%%%##%#%%\n"
				  "%#S%%\"%%'%&%'%&%'%&%&%&%'%&%##%#%%\n"
				  "%#y%r############y##########l#%S%%\n"
				  "%##%#######y###########y####lrq#%%\n"
				  "%#c%&&%'%&&%'%&&%'%&&%'%&&%'&%%%%%\n"
				  "%###y##Z####Z###################r%\n"
				  "%r#####ZZ###Z%\"%#y#%\"%##r%\"%#####%\n"
				  "%%%%&&%'&&%&&&#'&%''#&%&&+ +%%%%%%\n"
				  "ttttttttttttt%#%ttt%#%ttt+++\n"
				  "ttttttttttttt%S%ttt%S%\n"
				  "ttttttttttttt%#%ttt%#%\n"
				  "ttttttttttttt%#%ttt%#%\n"
				  "ttttttttttttt%S%ttt%S%\n"
				  "ttttttttttttt%#%ttt%#%\n"
				  "ttttttttttttt%\"%%%%%\"%\n"
				  "ttttt%%%%%%%%%###%###%%%%%%%%%\n"
				  "ttttt%xOOwwRww###%###wwRwwOOx%\n"
				  "ttttt%%x#########%#########x%%\n"
				  "ttttt%%%x#####cc#q######l#x%%%\n"
				  "ttttt%%%%x#######%##c####x%%%%\n"
				  "ttttt%'&'&'&'&&'&%&'&'&'&'&'&'";
	char	ex[] = "Gt'**-t&'.+\n"
				   "8t*+!*+!*+\n"
				   ";t&&'!&&'!&&'\n"
				   "9t(\n"
				   ">t7Ztfj^Ziu\n"
				   "Dt9dtndjt]ZVgt^i4\n"
				   "\n"
				   "L%t#$gZhdjgXZh$lVaah$lVaaThidcZTcZdc#ec\\\n"
				   "L&t#$gZhdjgXZh$lVaah$lVaaT)'TcZdc#ec\\\n"
				   "L't#$gZhdjgXZh$lVaah$lVaaT'#ec\\\n"
				   "L(t#$gZhdjgXZh$lVaah$lVaaTlddYTYVg`#ec\\\n"
				   "L)t#$gZhdjgXZh$lVaah$lVaaThidcZTVaeVXV#ec\\\n"
				   "L*t#$gZhdjgXZh$lVaah$lVaaThidcZT`Ve^WVgV#ec\\\n"
				   "L+t#$gZhdjgXZh$lVaah$lVaaThidcZTbZYdZY#ec\\\n"
				   "L,t#$gZhdjgXZh$lVaah$lVaaTbZiVaT&#ec\\\n"
				   "L-t#$gZhdjgXZh$lVaah$lVaaTbZiVaT'#ec\\\n"
				   "L.t#$gZhdjgXZh$lVaah$lVaaThidcZThVaVbVcYgV#ec\\\n"
				   "\n"
				   "L&%t#$gZhdjgXZh$lVaah$YddgTcZdcT%#ec\\\n"
				   "L&&t#$gZhdjgXZh$lVaah$YddgTcZdcT&#ec\\\n"
				   "L&'t#$gZhdjgXZh$lVaah$YddgTcZdcT'#ec\\\n"
				   "L&(t#$gZhdjgXZh$lVaah$YddgTcZdcT&#ec\\\n"
				   "\n"
				   "<%t#$gZhdjgXZh$lZVedch$`c^[ZTcZdcT\n"
				   "<&t#$gZhdjgXZh$lZVedch$e^hidaTcZdcT\n"
				   "<'t#$gZhdjgXZh$lZVedch$g^[aZT\n"
				   "\n"
				   "H%t#$gZhdjgXZh$heg^iZh$aVbe#ec\\\n"
				   "H&t#$gZhdjgXZh$heg^iZh$X]VcYZa^Zg#ec\\\n"
				   "H't#$gZhdjgXZh$heg^iZh$aVbeThiVcY#ec\\\n"
				   "H(t#$gZhdjgXZh$heg^iZh$eaVciT\\gZZc#ec\\\n"
				   "H)t#$gZhdjgXZh$heg^iZh$eaVciTYgn#ec\\\n"
				   "H*t#$gZhdjgXZh$heg^iZh$kVhZ#ec\\\n"
				   "H+t#$gZhdjgXZh$heg^iZh$[aV\\#ec\\\n"
				   "H,t#$gZhdjgXZh$heg^iZh$WVggZaTWgdlc#ec\\\n"
				   "H-t#$gZhdjgXZh$heg^iZh$WVggZaT\\gZZc#ec\\\n"
				   "H.t#$gZhdjgXZh$heg^iZh$iVWaZ#ec\\\n"
				   "H&%t#$gZhdjgXZh$heg^iZh$iVWaZTX]V^gh#ec\\\n"
				   "H&&t#$gZhdjgXZh$heg^iZh$hiVijZ#ec\\\n"
				   "H&'t#$gZhdjgXZh$heg^iZh$lZaaTZbein#ec\\\n"
				   "H&(t#$gZhdjgXZh$heg^iZh$lZaaT[jaa#ec\\\n"
				   "H&)t#$gZhdjgXZh$heg^iZh$h`ZaZidc#ec\\\n"
				   "H&*t#$gZhdjgXZh$heg^iZh$WdcZh#ec\\\n"
				   "H&+t#$gZhdjgXZh$heg^iZh$ejYYaZ#ec\\\n"
				   "H&,t#$gZhdjgXZh$heg^iZh$edi#ec\\\n"
				   "H&-t#$gZhdjgXZh$heg^iZh$\\jVgYTYZVY#ec\\\n"
				   "H&.t#$gZhdjgXZh$heg^iZh$]ZVai]Tma#ec\\\n"
				   "H'%t#$gZhdjgXZh$heg^iZh$]ZVai]Ta#ec\\\n"
				   "H'&t#$gZhdjgXZh$heg^iZh$]ZVai]Tb#ec\\\n"
				   "H''t#$gZhdjgXZh$heg^iZh$\\jcTg^[aZ#ec\\\n"
				   "H'(t#$gZhdjgXZh$heg^iZh$Vbbd#ec\\\n"
				   "H')t#$gZhdjgXZh$heg^iZh$WdcjhTma#ec\\\n"
				   "H'*t#$gZhdjgXZh$heg^iZh$WdcjhTa#ec\\\n"
				   "H'+t#$gZhdjgXZh$heg^iZh$WdcjhTb#ec\\\n"
				   "H',t#$gZhdjgXZh$heg^iZh$WdcjhTh#ec\\\n"
				   "\n"
				   "B%t#$gZhdjgXZh$bjh^X$%&t6gdjcYti]Zt8dgcZgu#lVk\n"
				   ":%t#$gZhdjgXZh$ZcZb^Zh$bjiVci$\n"
				   "\n"
				   "6%t#$gZhdjgXZh$hdjcYh$eaVnZgTcdVXi^dc#lVk\n"
				   "6&t#$gZhdjgXZh$hdjcYh$eaVnZgTeV^c#lVk\n"
				   "6't#$gZhdjgXZh$hdjcYh$eaVnZgTYZVi]#lVk\n"
				   "6(t#$gZhdjgXZh$hdjcYh$YddgTdeZc#lVk\n"
				   "6)t#$gZhdjgXZh$hdjcYh$YddgTXadhZ#lVk\n"
				   "6*t#$gZhdjgXZh$hdjcYh$lZVedcT`c^[Z#lVk\n"
				   "6+t#$gZhdjgXZh$hdjcYh$lZVedcTe^hida#lVk\n"
				   "6,t#$gZhdjgXZh$hdjcYh$lZVedcTg^[aZ#lVk\n"
				   "6-t#$gZhdjgXZh$hdjcYh$e^X`jeTVbbd#lVk\n"
				   "6.t#$gZhdjgXZh$hdjcYh$e^X`jeTg^[aZ#lVk\n"
				   "6&%t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tb#lVk\n"
				   "6&&t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Ta#lVk\n"
				   "6&'t#$gZhdjgXZh$hdjcYh$e^X`jeT]ZVai]Tma#lVk\n"
				   "6&(t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTh#lVk\n"
				   "6&)t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTb#lVk\n"
				   "6&*t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTa#lVk\n"
				   "6&+t#$gZhdjgXZh$hdjcYh$e^X`jeTWdcjhTma#lVk\n"
				   "\n"
				   "tttttttttttttttttttttttttttttttttttt%%%%%%%%%%%%&%%%%%%%%%%%%\n"
				   "tttttttttttttt,,-,-,,ttttttttttttttt%#######################%\n"
				   "tttttttttttttt,##r##,ttttttttttttttt%##~########~########~##%\n"
				   "tt,,,,,,,,,,,,,##S##,ttttttttttttttt%####)%%%%/###/%%*3*####%\n"
				   "tt,ccccccccccc,#####,ttttttttttttttt%####k##&%##w##%&##%####%\n"
				   "tt,ccccccccccc,,,3,,,,,,,,,,,,,,,,,%%####)#S#%x###x%#S#%####%\n"
				   "%-,ccccccccccc,####0,cccccccccccccc,%####%&#-%##w##%-s&%####%\n"
				   "-:k###S###S###k##S##k###############k####%%)%%##~##%%*%%####%\n"
				   "%-,hhhhhhhhhhh,#####,hhhhhhhhhhh,#,,%##~######## ########~##%\n"
				   "tt,hhhhhhhhhhh,####0,hhhhhhhhhhh,#Z,%####%%+%%##~##%%.%%####%\n"
				   "tt,hhhhhhhhhhh,,,3,,,hhhhhhhhhhh,#Z,%####%&#-%##w##%-#&%####%\n"
				   "tt,,,,,,,,,,,,,#####,hhhhhhhhhhh,cZ,%####%#S#%x###x%#S#.####%\n"
				   "tttttttttttttt,##S##,,,,,,,,,,,,,,,,%####%##&%##w##%&##k####%\n"
				   "tttttttttttttt,TT#V5,ttttttttttttttt%####+3+%%/###/%%%%.####%\n"
				   "tttttttttttttt,,-,-,,ttttttttttttttt%##~########~########~##%\n"
				   "tttttttttttttttttttttttttttttttttttt%#######################%\n"
				   "tttttttttttttttttttttttttttttttttttt%%%%%%%%%%%%&%%%%%%%%%%%%";
	char	*target;

	target = NULL;
	if (av == 829239653)
		target = e;
	else if (av == 1852794222)
		target = n;
	else if (av == 1920102248)
		target = h;
	else if (av == 1953069157)
	{
		target = ex;
		game->lastlevel = true;
	}
	else
		terminate(game, ERR_ARGS, "Wrong scene filename");
	if ((file_id = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	for (unsigned i = 0; i < ft_strlen(target); ++i)
	{
		if (target[i] == '\n')
			write(file_id, "\n", 1);
		else
		{
			target[i] = (target[i] - 32 + 95 - 84) % 95 + 32;
			write(file_id, target + i, 1);
		}
	}
	close(file_id);
}

void	parse(int args, char **av, t_game *game, bool *screenshot_only)
{
	int		file_id;
	char	*line;

	if (args == 1)
		terminate(game, ERR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(game, ERR_ARGS, "Too many arguments");
	av++;
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(game, ERR_ARGS, "Wrong scene filename");

//	encode();
//	encode_wav();
	generate_scenefile(game, *(unsigned *)*av);
	if ((file_id = open(".tmp", O_RDONLY)) == -1)
//	if ((file_id = open(*av, O_RDONLY)) == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	validate_settings(game);
	parse_map(file_id, line, game->objects, game);
	if (close(file_id) == -1)
		terminate(game, ERR_PARSE, strerror(errno));

	file_id = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	close(file_id);

	*screenshot_only = false;
	if (args == 3)
	{
		if (ft_strncmp("--save", *(av + 1), 7) == 0)
			*screenshot_only = true;
		else
			terminate(game, ERR_ARGS, "Invalid option");
	}
	load_wav(game);
}

void	set_enemies(char *string, t_game *g)
{
	unsigned	id;
	int			i;
	char		*path;

	if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
		terminate(g, ERR_PARSE, "Enemy ID is wrong (Exx)");
	if (id >= sizeof(g->enemyset) / sizeof(*g->enemyset))
		terminate(g, ERR_PARSE, "Enemy ID out of array range");
	if (g->enemyset[id].death[0].ptr != NULL)
		terminate(g, ERR_PARSE, "Duplicated enemy spriteset setting");
	if ((path = ft_strjoin(string, "wait_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].wait, 8, path, g);
	if ((path = ft_strjoin(string, "attack_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].attack, 3, path, g);
	if ((path = ft_strjoin(string, "death_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].death, 5, path, g);
	if ((path = ft_strjoin(string, "pain_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].pain, 2, path, g);
	load_audioset(&g->enemyset[id], string, g);
	i = 0;
	while (i < 8)
	{
		if ((path = ft_strjoin(string,
					(char []){'w', 'a', 'l', 'k', '_', '0' + i, '\0'})) == NULL)
			terminate(g, ERR_PARSE, strerror(errno));
		load_spriteset(g->enemyset[id].walk[i], 4, path, g);
		i++;
	}
}

void	set_audio(char *string, t_game *game)
{
	unsigned	id;

	if (*string == 'M')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Music ID is wrong (Mxx)");
		if (id >= sizeof(game->audio.music) / sizeof(*game->audio.music))
			terminate(game, ERR_PARSE, "Music ID out of array range");
		if (game->audio.music[id].file.channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated music setting");
		 load_audio_file(&game->audio.music[id], string);
	}
	else if (*string == 'A')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Sound ID is wrong (Axx)");
		if (id >= sizeof(game->audio.sound) / sizeof(*game->audio.sound))
			terminate(game, ERR_PARSE, "Sound ID out of array range");
		if (game->audio.sound[id].file.channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated sound setting");
		load_audio_file(&game->audio.sound[id], string);
	}
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1U;
	game->color_ceil = -1U;
	while ((status = get_next_line(file_id, line)) >= 0)
	{
		if (**line == 'R')
			set_resolution(*line, &game->img.size, game);
		else if (**line == 'C')
			set_colors(*line, &game->color_ceil, game);
		else if (**line == 'F')
			set_colors(*line, &game->color_floor, game);
		else if (**line == 'W' || **line == 'S')
			set_textures(*line, game);
		else if (**line == 'G')
			set_weapons(*line, game);
		else if (**line == 'E')
			set_enemies(*line, game);
		else if (**line == 'M' || **line == 'A')
			set_audio(*line, game);
		else if (**line == 'D')
			atoi_limited(&game->fade_distance, *line + 1, UINT_MAX);
		else if (**line == 'I' && game->string.text == NULL)
			string_add(game, ft_strdup(*line + 2), 5, COLOR_WHITE);
		else if (**line == 'O' && game->endleveltext == NULL)
			game->endleveltext = ft_strdup(*line + 2);
		else if (**line != '#' && **line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(game, ERR_PARSE, "There is no map in scene file");
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
}

void	parse_map(int file_id, char *line, t_list *map, t_game *game)
{
	t_list		*line_lst;
	int			status;
	unsigned	line_len;

	if ((map = ft_lstnew(line)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map first row)");
	game->map.size = (t_upoint){ft_strlen(line), 1};
	while ((status = get_next_line(file_id, &line)) >= 0 && *line != '\0')
	{
		if ((line_lst = ft_lstnew(line)) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
		ft_lstadd_front(&map, line_lst);
		if (game->map.size.x < (line_len = ft_strlen(line)))
			game->map.size.x = line_len;
		game->map.size.y++;
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
	free(line);
	if (status != 0)
		terminate(game, ERR_PARSE, "Empty lines in map are not allowed");
	if ((game->map.grid = malloc(sizeof(char *) * game->map.size.y)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
	set_map(game, map);
}

void	validate_settings(t_game *game)
{
	unsigned	i;

	if (game->color_floor == -1U)
		terminate(game, ERR_PARSE, "Floor color not found. Format: F R,G,B");
	if (game->color_ceil == -1U)
		terminate(game, ERR_PARSE, "Ceil color not found. Format: C R,G,B");
	if (game->img.size.x == 0 || game->img.size.y == 0)
		terminate(game, ERR_PARSE,
		"Resolution doesn't set. Format: 'R WIDTH HEIGHT'");
	i = 0;
	while (i < sizeof(game->texture) / sizeof(*game->texture) / 2)
		if (game->texture[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing wall texture");
	i = 0;
	while (i < sizeof(game->sprite) / sizeof(*game->sprite))
		if (game->sprite[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing sprite");
	i = 0;
	while (i < sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		if (game->p.weapon_img[i++][0].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing weapon texture");
}

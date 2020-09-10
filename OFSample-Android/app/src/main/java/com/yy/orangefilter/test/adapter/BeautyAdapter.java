package com.yy.orangefilter.test.adapter;

import android.view.View;
import android.widget.ImageView;

import com.yy.orangefilter.test.R;
import com.yy.orangefilter.test.base.BaseAdapter;
import com.yy.orangefilter.test.bean.BeautyBean;

import org.jetbrains.annotations.NotNull;

import java.util.List;

/**
 * 美颜
 */
public class BeautyAdapter extends BaseAdapter<BeautyBean> {

    private List<BeautyBean> datas;

    public BeautyAdapter(List<BeautyBean> datas) {
        super(datas);
        this.datas = datas;
    }

    @Override
    public int getLayoutId(int viewType) {
        return R.layout.layout_menu_item;
    }

    @Override
    public void onCreateViewHolder(@NotNull VH holder, int viewType) {

    }

    @Override
    public void onBindViewHolder(@NotNull VH holder, BeautyBean data, int position) {
        holder.setText(R.id.tvItem, data.getOptionName());
        holder.setImageRes(R.id.ivItem, data.getThumb());
        ImageView borderView = holder.getView(R.id.ivBorder);

        if (data.isEnable()) {
            borderView.setVisibility(View.VISIBLE);
        } else {
            borderView.setVisibility(View.INVISIBLE);
        }
    }

    public void selectItem(int position) {
        resetItem();
        datas.get(position).setEnable(true);
        notifyDataSetChanged();
    }

    public void resetItem() {
        for (int i = 0; i < datas.size(); i++) {
            datas.get(i).setEnable(false);
        }
        notifyDataSetChanged();
    }


}
